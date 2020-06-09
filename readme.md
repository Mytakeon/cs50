Repository to develop and run CS50 assignments locally.

## Installation

Prerequisite: clang, VScode, Python 3

Install `astyle` (only required by `style50`):
```bash
brew install astyle  # macOS 
apt-get astyle  # Ubuntu
```

Install CS50's python packages, to be able to run `check50`, `style50`, `submit50`:
```bash
python3 -m venv venv
source venv/bin/activate  # macOS
pip3 install -r requirements.txt
```

## Usage

Use the existing configurations in `.vscode` to compile, run and debug your code.
To compile the file you are currently editing, you can use `cmd/ctrl + shift + B`.
If arguments are required when debugging, add them in `.vscode/launch.json`, in the `args` list.

---

Sources:
* https://github.com/cs50/libcs50