# Basics
autoload -U compinit; compinit

# Customization: prompt
MYHOST=""
if [[ "$OSTYPE" == "darwin"* ]]; then
  MYHOST="$(networksetup -getcomputername)"
  MYHOST=${MYHOST// /-}
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  MYHOST="$(hostname)"
fi

PS1="%F{green}%n%f%F{green}@%f%F{green}${MYHOST}%f:%F{magenta}%~%f$ "
export CLICOLOR=1

# Initialization: VSCode
if [[ "$OSTYPE" == "darwin"* ]]; then
  export PATH="/Applications/Visual Studio Code.app/Contents/Resources/app/bin:$PATH"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  export PATH="/usr/share/code/bin:$PATH"
fi

# Initialization: GPG
if ! gpgconf --list-dirs agent-socket >/dev/null 2>&1; then
  gpgconf --launch gpg-agent
fi

GPG_TTY=$(tty)
export GPG_TTY

# Alias: ls <-> eza
alias ls="eza"
alias ll="eza --long --header --classify"
alias la="eza --long --header --classify --all"
alias lt="eza --tree --level=1"
alias ltt="eza --tree --level=2"
alias lttt="eza --tree --level=3"

# Alias: nvim <-> neovim
if [[ "$OSTYPE" == "darwin"* ]]; then
  alias vi="nvim"
  alias neovim="nvim"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  alias vi="neovim"
  alias nvim="neovim"
fi


# Extension: zsh-syntax-highlighting
if [[ "$OSTYPE" == "darwin"* ]]; then
  source /opt/homebrew/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  source /usr/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
fi

# Extension: zsh-autosuggestions
if [[ "$OSTYPE" == "darwin"* ]]; then
	source /opt/homebrew/share/zsh-autosuggestions/zsh-autosuggestions.zsh
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
	source /usr/share/zsh-autosuggestions/zsh-autosuggestions.zsh
fi
