section "init", ROM0[$100]
jp main

section "main", ROM0[$150]
main:
    ld hl, 100
    ld [HLD],A
    halt