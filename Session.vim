let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/assig/pro2/EntregaFinal
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
argglobal
%argdel
$argadd cjt_clusters.cc
$argadd cjt_especies.cc
$argadd cluster.cc
$argadd especie.cc
$argadd program.cc
$argadd BinTree.hh
$argadd cjt_clusters.hh
$argadd cjt_especies.hh
$argadd cluster.hh
$argadd especie.hh
edit cjt_clusters.cc
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 102 - ((14 * winheight(0) + 18) / 37)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
102
normal! 05|
tabnext 1
badd +0 cjt_clusters.cc
badd +0 cjt_especies.cc
badd +0 cluster.cc
badd +0 especie.cc
badd +0 program.cc
badd +0 BinTree.hh
badd +0 cjt_clusters.hh
badd +0 cjt_especies.hh
badd +0 cluster.hh
badd +0 especie.hh
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToOSc
set winminheight=1 winminwidth=1
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
nohlsearch
let g:this_session = v:this_session
let g:this_obsession = v:this_session
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
