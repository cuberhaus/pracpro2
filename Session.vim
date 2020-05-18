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
$argadd BinTree.hh
$argadd cjt_clusters.hh
$argadd cjt_especies.hh
$argadd cluster.hh
$argadd especie.hh
$argadd cjt_clusters.cc
$argadd cjt_especies.cc
$argadd cluster.cc
$argadd especie.cc
$argadd program.cc
edit program.cc
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
argglobal
if bufexists("program.cc") | buffer program.cc | else | edit program.cc | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 178 - ((41 * winheight(0) + 21) / 42)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
178
normal! 0
tabnext 1
badd +71 BinTree.hh
badd +1 cjt_clusters.hh
badd +14 cjt_especies.hh
badd +25 cluster.hh
badd +27 especie.hh
badd +108 cjt_clusters.cc
badd +107 cjt_especies.cc
badd +25 cluster.cc
badd +1 especie.cc
badd +1 program.cc
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
