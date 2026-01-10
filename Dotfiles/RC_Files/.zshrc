# Common
autoload -Uz compinit
compinit
autoload -Uz vcs_info

zstyle ':vcs_info:*' enable git svn
zstyle ':vcs_info:git:*' formats $'\uE725 %b'

export CLICOLOR=1

## Customization: prompt
MYHOST=""
if [[ "$OSTYPE" == "darwin"* ]]; then
  MYHOST="$(networksetup -getcomputername)"
  MYHOST=${MYHOST// /-}
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  MYHOST="$(hostname)"
fi

precmd() {
  vcs_info

  if [[ -z ${vcs_info_msg_0_} ]]; then
    # nothing from vcs_info
    PS1="%F{green}%n%f%F{green}@%f%F{green}${MYHOST}%f:%F{magenta}%~%f$ "
  else
    # vcs_info found something
    PS1="%F{green}%n%f%F{green}@%f%F{green}${MYHOST}%f:%F{magenta}%~%f %F{cyan}${vcs_info_msg_0_}%f$ "
  fi

  # Time on the right (light grey)
  RPROMPT='%F{245}%D{%l:%M:%S %p}%f'
}

## Initialization: VSCode
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

## Alias: ls <-> exa
alias ls="eza"
alias ll="eza --long --header --classify"
alias la="eza --long --header --classify --all"
alias lt="eza --tree --level=1"
alias ltt="eza --tree --level=2"
alias lttt="eza --tree --level=3"

## Alias: nvim <-> neovim
if [[ "$OSTYPE" == "darwin"* ]]; then
  alias vi="nvim"
  alias neovim="nvim"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  alias vi="neovim"
  alias nvim="neovim"
fi

## Extension: zsh-syntax-highlighting
if [[ "$OSTYPE" == "darwin"* ]]; then
  source /opt/homebrew/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  source /usr/share/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh
fi

## Extension: zsh-autosuggestions
if [[ "$OSTYPE" == "darwin"* ]]; then
  source /opt/homebrew/share/zsh-autosuggestions/zsh-autosuggestions.zsh
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
  source /usr/share/zsh-autosuggestions/zsh-autosuggestions.zsh
fi

# Machine-specific configs

## e2fsprogs
export PATH="/opt/homebrew/opt/e2fsprogs/bin:$PATH"
export PATH="/opt/homebrew/opt/e2fsprogs/sbin:$PATH"
