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
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
set nosplitbelow
set nosplitright
wincmd t
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 20 + 22) / 45)
exe 'vert 1resize ' . ((&columns * 79 + 79) / 158)
exe '2resize ' . ((&lines * 21 + 22) / 45)
exe 'vert 2resize ' . ((&columns * 79 + 79) / 158)
exe '3resize ' . ((&lines * 21 + 22) / 45)
exe 'vert 3resize ' . ((&columns * 78 + 79) / 158)
exe '4resize ' . ((&lines * 20 + 22) / 45)
exe 'vert 4resize ' . ((&columns * 78 + 79) / 158)
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
let s:l = 145 - ((9 * winheight(0) + 10) / 20)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
145
normal! 0
wincmd w
argglobal
if bufexists("cluster.cc") | buffer cluster.cc | else | edit cluster.cc | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 5 - ((3 * winheight(0) + 10) / 21)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
5
normal! 05|
wincmd w
argglobal
if bufexists("BinTree.hh") | buffer BinTree.hh | else | edit BinTree.hh | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 61 - ((15 * winheight(0) + 10) / 21)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
61
normal! 0
wincmd w
argglobal
if bufexists("cjt_clusters.cc") | buffer cjt_clusters.cc | else | edit cjt_clusters.cc | endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 12 - ((9 * winheight(0) + 10) / 20)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
12
normal! 05|
wincmd w
2wincmd w
exe '1resize ' . ((&lines * 20 + 22) / 45)
exe 'vert 1resize ' . ((&columns * 79 + 79) / 158)
exe '2resize ' . ((&lines * 21 + 22) / 45)
exe 'vert 2resize ' . ((&columns * 79 + 79) / 158)
exe '3resize ' . ((&lines * 21 + 22) / 45)
exe 'vert 3resize ' . ((&columns * 78 + 79) / 158)
exe '4resize ' . ((&lines * 20 + 22) / 45)
exe 'vert 4resize ' . ((&columns * 78 + 79) / 158)
tabnext 1
badd +48 BinTree.hh
badd +8 cjt_clusters.hh
badd +17 cjt_especies.hh
badd +30 cluster.hh
badd +57 especie.hh
badd +28 cjt_clusters.cc
badd +52 cjt_especies.cc
badd +5 cluster.cc
badd +40 especie.cc
badd +145 program.cc
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
let g:this_session = v:this_session
let g:this_obsession = v:this_session
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
