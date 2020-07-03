Repository to develop and run the [2020 CS50](https://cs50.harvard.edu/x/2020/) assignments locally.

## Installation

Prerequisite: [VScode](https://code.visualstudio.com/), Python 3

- For week 1 to 6:

  - `clang`, e.g. with brew: `brew install llvm`
  - `astyle` (only required by `style50`), e.g. `brew install astyle`

  Install CS50's python packages, to be able to run `check50`, `style50`, `submit50`, and use `cs50` :

  ```bash
  python3 -m venv venv
  source venv/bin/activate
  pip3 install -r requirements.txt
  ```

- Week 7:
  - [DB Browser](https://sqlitebrowser.org/dl/) for SQLite3, e.g. `brew cask install db-browser-for-sqlite`

## Usage

Use the existing configurations in `.vscode` to compile, run and debug your code.
To compile the file you are currently editing, you can use `cmd/ctrl + shift + B`.
If arguments are required when debugging, add them in `.vscode/launch.json`, in the `args` list.
