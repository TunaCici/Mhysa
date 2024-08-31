syntax on

set number
set relativenumber
set tabstop=8
set shiftwidth=8
set softtabstop=8
set autoindent
set smartindent

autocmd BufReadPost *
  \ if line("'\"") > 1 && line("'\"") <= line("$") |
  \   exe "normal! g`\"" |
  \ endif

