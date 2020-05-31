Repository to develop and run CS50 assignments locally.

## Installation

Prerequisite: GCC, VScode, Python 3

Install `astyle` (only required by `style50`):
```bash
brew install astyle  # macOS 
apt-get astyle  # Ubuntu
```

Install CS50's python packages, to be able to run `check50` and `style50`:
```bash
python3 -m venv my_venv
source my_venv/bin/activate  # macOS
pip3 install -r requirements.txt
```

## Usage

Use the existing configurations in `.vscode` to compile, run and debug your code.
If arguments are required, add them in `.vscode/launch.json`, in the `args` list.

---

Sources:
* https://github.com/cs50/libcs50