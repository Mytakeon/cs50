import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    assets = db.execute("SELECT * FROM assets WHERE user_id = :id", id=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
    total = cash
    for asset in assets:
        quote = lookup(asset["symbol"])
        asset["price"] = usd(quote["price"])
        asset_total = quote["price"] * asset["quantity"]
        asset["total"] = usd(asset_total)
        total += asset_total

    return render_template("assets.html", assets=assets, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    ticker = request.form.get("symbol")
    shares = request.form.get("shares")

    if not ticker or not shares:
        return apology("must provide both symbol and shares", 403)

    quote = lookup(ticker)
    if not quote:
        return apology(f"Ticker '{ticker}' not found")

    try:
        quantity = int(shares)
    except ValueError:
        return apology("Shares must be a positive integer")

    rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    cash = rows[0]["cash"]

    total = quantity * quote["price"]
    if cash < total:
        return apology("you don't have enough cash", 403)

    # this should all be in a single transaction to be atomic
    db.execute("UPDATE users SET cash = :cash WHERE id= :id", id=session["user_id"], cash=cash - total)
    db.execute(
        "INSERT INTO transactions (user_id, symbol, type, quantity, share_price) VALUES (:id, :symbol, :type, :quantity, :share_price)",
        id=session["user_id"], symbol=quote["symbol"], quantity=quantity, share_price=quote["price"], type="buy")
    db.execute(
        "INSERT INTO assets (user_id, symbol, quantity) VALUES (:id, :symbol, :quantity) ON CONFLICT(symbol) DO UPDATE SET quantity=quantity + :quantity",
        id=session["user_id"], symbol=quote["symbol"], quantity=quantity)

    flash(f"Bought {quantity} {quote['symbol']} at {quote['price']}")

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :id", id=session["user_id"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        if not username:
            return apology("must provide username", 403)

        elif not password:
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        flash(f"Logged in as '{username}'")
        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("get_quote.html")

    ticker = request.form.get("quote")
    quote = lookup(ticker)
    if not quote:
        return apology(f"Ticker '{ticker}' not found")

    return render_template("quote.html", ticker=f"{quote['name']} ({ticker})", quote=usd(quote['price']))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")

    if request.method == "POST":
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 403)

        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=username)
        if len(rows) == 1:
            return apology("This username is already taken", 403)

        password = request.form.get("password")
        if not password:
            return apology("must provide password", 403)

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                   username=username, hash=generate_password_hash(password))

        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        symbols = db.execute("SELECT symbol FROM assets WHERE user_id = :id", id=session["user_id"])
        if not symbols:
            return apology("You don't have any stocks to sell")
        return render_template("sell.html", tickers=[symbol["symbol"] for symbol in symbols])

    ticker = request.form.get("symbol")
    shares = request.form.get("shares")

    if not ticker or not shares:
        return apology("Both symbol and shares must be specified")

    try:
        quantity = int(shares)
    except ValueError:
        return apology("Shares must be a positive integer")

    # Make the request again to make sure it is up to date
    assets = db.execute("SELECT * FROM assets WHERE user_id = :id", id=session["user_id"])
    sell_price = None
    for asset in assets:
        if asset["symbol"] == ticker:
            sell_price = lookup(asset["symbol"])["price"]
            owned_shares = asset["quantity"]
            continue

    if not sell_price:  # the stock was not in the assets
        return apology(f"You don't own '{ticker}'")

    if owned_shares < quantity:
        return apology(f"You cannot sell {quantity} shares, you only own {owned_shares} shares.")

    # this should all be in a single transaction to be atomic
    if owned_shares == quantity:
        db.execute("DELETE from assets WHERE user_id=:id AND symbol =:symbol",
                   id=session["user_id"], symbol=ticker)
    else:
        db.execute(
            "UPDATE assets SET quantity = quantity - :quantity WHERE id = :id AND symbol = :symbol",
            id=session["user_id"], symbol=ticker, quantity=quantity)

    db.execute("UPDATE users SET cash = cash + :cash WHERE id= :id", id=session["user_id"], cash=quantity * sell_price)
    db.execute(
        "INSERT INTO transactions (user_id, symbol, type, quantity, share_price) VALUES (:id, :symbol, :type, :quantity, :share_price)",
        id=session["user_id"], symbol=ticker, quantity=quantity, share_price=sell_price, type="sell")

    flash(f"Sold {quantity} share{'s' if quantity > 1 else ''} of '{ticker}' ")

    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
