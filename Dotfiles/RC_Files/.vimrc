filetype plugin indent on                                                       
syntax on                                                                       
                                                                                
set number                                                                      
set relativenumber                                                              
                                                                                
set tabstop=8                                                                   
set shiftwidth=8                                                                
set softtabstop=8                                                               
                                                                                
set autoindent                                                                  
set smartindent                                                                 
                                                                                
set wildmenu                                                                    
set wildmode=longest:full,full                                                  
                                                                                
set colorcolumn=80                                                              
                                                                                
" Remember last cursor loc                                                      
autocmd BufReadPost *                                                           
  \ if line("'\"") > 1 && line("'\"") <= line("$") |                            
  \   exe "normal! g`\"" |                                                      
  \ endif
