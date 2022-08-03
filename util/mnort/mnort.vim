command! -nargs=1 Silent
            \   execute 'silent !' . <q-args>
            \ | execute 'redraw!'

nnoremap <leader>s vit!mnort client <CR><c-o>
nnoremap <silent> <leader>p :Silent echo 'lil("playtog")' \| mnort client <CR>
