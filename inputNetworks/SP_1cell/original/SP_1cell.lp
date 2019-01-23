edge(en, ci1, 0).

functionOr(ci1, 1).
functionAnd(ci1, 1, en).

edge(ci1, ciact_b1, 1).
edge(pka_b1, ciact_b1, 0).
edge(ciact_b1, ciact_b1, 1).
edge(ciact_b2, ciact_b1, 1).
edge(dsh, ciact_b1, 1).

functionOr(ciact_b1, 1..3).
functionAnd(ciact_b1, 1, ci1).
functionAnd(ciact_b1, 1, pka_b1).
functionAnd(ciact_b1, 2, ciact_b1).
functionAnd(ciact_b1, 2, ciact_b2).
functionAnd(ciact_b1, 3, ci1).
functionAnd(ciact_b1, 3, dsh).

edge(dsh, ciact_b2, 1).
edge(ci1, ciact_b2, 1).
edge(ciact_b1, ciact_b2, 1).
edge(pka_b1, ciact_b2, 0).

functionOr(ciact_b2, 1).
functionAnd(ciact_b2, 1, dsh).
functionAnd(ciact_b2, 1, ci1).
functionAnd(ciact_b2, 1, ciact_b1).
functionAnd(ciact_b2, 1, pka_b1).

edge(dsh, cirep, 0).
edge(ci1, cirep, 1).
edge(pka_b1, cirep, 1).
edge(pka_b2, cirep, 1).

functionOr(cirep, 1).
functionAnd(cirep, 1, dsh).
functionAnd(cirep, 1, ci1).
functionAnd(cirep, 1 ,pka_b1).
functionAnd(cirep, 1, pka_b2).

edge(fz, dsh, 1).

functionOr(dsh, 1).
functionAnd(dsh, 1, fz).

edge(dsh, en, 1).
edge(slp, en, 0).

functionOr(en, 1).
functionAnd(en, 1, dsh).
functionAnd(en, 1, slp).

edge(wg_ext, fz, 1).
edge(wg_b1, fz, 1).

functionOr(fz, 1..2).
functionAnd(fz, 1, wg_ext).
functionAnd(fz, 2, wg_b1).

edge(en, hh, 1).
edge(cirep, hh, 0).

functionOr(hh, 1).
functionAnd(hh, 1, en).
functionAnd(hh, 1, cirep).

edge(hh_ext, hh_ext, 1).

functionOr(hh_ext, 1).
functionAnd(hh_ext, 1, hh_ext).

edge(nkd_b1, nkd_b1, 1).
edge(nkd_b2, nkd_b1, 1).
edge(en, nkd_b1, 0).

functionOr(nkd_b1, 1..2).
functionAnd(nkd_b1, 1, nkd_b1).
functionAnd(nkd_b1, 1, nkd_b2).
functionAnd(nkd_b1, 2, en).

edge(dsh, nkd_b2, 1).
edge(nkd_b1, nkd_b2, 1).
edge(en, nkd_b2, 0).

functionOr(nkd_b2, 1).
functionAnd(nkd_b2, 1, dsh).
functionAnd(nkd_b2, 1, nkd_b1).
functionAnd(nkd_b2, 1, en).

edge(pka_b1, pka_b1, 1).
edge(pka_b2, pka_b1, 1).
edge(ptc_b1, pka_b1, 1).

functionOr(pka_b1, 1..2).
functionAnd(pka_b1, 1, pka_b1).
functionAnd(pka_b1, 1, pka_b2).
functionAnd(pka_b1, 2, ptc_b1).

edge(pka_b1, pka_b2, 1).
edge(ptc_b1, pka_b2, 1).

functionOr(pka_b2, 1).
functionAnd(pka_b2, 1, pka_b1).
functionAnd(pka_b2, 1, ptc_b1).

edge(en, ptc_b1, 0).
edge(hh_ext, ptc_b1, 0).
edge(ptc_b1, ptc_b1, 1).
edge(ptc_b2, ptc_b1, 1).

functionOr(ptc_b1, 1..2).
functionAnd(ptc_b1, 1, en).
functionAnd(ptc_b1, 1, hh_ext).
functionAnd(ptc_b1, 2, ptc_b1).
functionAnd(ptc_b1, 2, ptc_b2).

edge(en, ptc_b2, 0).
edge(ciact_b1, ptc_b2, 1).
edge(ptc_b1, ptc_b2, 1).
edge(hh_ext, ptc_b2, 0).

functionOr(ptc_b2, 1).
functionAnd(ptc_b2, 1, en).
functionAnd(ptc_b2, 1, ciact_b1).
functionAnd(ptc_b2, 1, ptc_b1).
functionAnd(ptc_b2, 1, hh_ext).

edge(dsh, slp, 1).
edge(en, slp, 0).

functionOr(slp, 1).
functionAnd(slp, 1, dsh).
functionAnd(slp, 1, en).

edge(slp, wg_b1, 1).
edge(nkd_b1, wg_b1, 0).
edge(en, wg_b1, 0).
edge(ciact_b1, wg_b1, 1).
edge(nkd_b2, wg_b1, 0).
edge(ciact_b2, wg_b1, 1).
edge(wg_b1, wg_b1, 1).
edge(wg_b2, wg_b1, 1).

functionOr(wg_b1, 1..4).
functionAnd(wg_b1, 1, slp).
functionAnd(wg_b1, 1, nkd_b1).
functionAnd(wg_b1, 1, en).
functionAnd(wg_b1, 1, ciact_b1).
functionAnd(wg_b1, 2, slp).
functionAnd(wg_b1, 2, en).
functionAnd(wg_b1, 2, ciact_b1).
functionAnd(wg_b1, 2, nkd_b2).
functionAnd(wg_b1, 3, slp).
functionAnd(wg_b1, 3, en).
functionAnd(wg_b1, 3, ciact_b1).
functionAnd(wg_b1, 3, ciact_b2).
functionAnd(wg_b1, 4, wg_b1).
functionAnd(wg_b1, 4, wg_b2).

edge(wg_b1, wg_b2, 1).
edge(slp, wg_b2, 1).
edge(en, wg_b2, 0).
edge(ciact_b1, wg_b2, 1).
edge(ciact_b2, wg_b2, 1).

functionOr(wg_b2, 1).
functionAnd(wg_b2, 1, wg_b1).
functionAnd(wg_b2, 1, slp).
functionAnd(wg_b2, 1, en).
functionAnd(wg_b2, 1, ciact_b1).
functionAnd(wg_b2, 1, ciact_b2).

edge(wg_ext, wg_ext, 1).

functionOr(wg_ext, 1).
functionAnd(wg_ext, 1, wg_ext).

%exp(1).