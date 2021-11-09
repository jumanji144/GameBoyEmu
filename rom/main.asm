section "vectors", ROM0[$0000]
jp main

section "init", ROM0[$100]
jp main

section "main", ROM0[$150]
main:
    ld a, $FF
    ld [bc], a

section "success", ROM0[$200]
success:
    halt
