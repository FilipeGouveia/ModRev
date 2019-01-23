edge(fos, ap1, 1).
edge(jun, ap1, 1).

functionOr(ap1, 1).
functionAnd(ap1, 1, fos).
functionAnd(ap1, 1, jun).

edge(cd45, cd45, 1).

functionOr(cd45, 1).
functionAnd(cd45, 1, cd45).

edge(cd8, cd8, 1).

functionOr(cd8, 1).
functionAnd(cd8, 1, cd8).

edge(creb, cre, 1).

functionOr(cre, 1).
functionAnd(cre, 1, creb).

edge(rsk, creb, 1).

functionOr(creb, 1).
functionAnd(creb, 1, rsk).

edge(ip3, ca, 1).

functionOr(ca, 1).
functionAnd(ca, 1, ip3).

edge(ca, calcin, 1).

functionOr(calcin, 1).
functionAnd(calcin, 1, ca).

edge(plcg_a, dag, 1).

functionOr(dag, 1).
functionAnd(dag, 1, plcg_a).

edge(mek, erk, 1).

functionOr(erk, 1).
functionAnd(erk, 1, mek).

edge(erk, fos, 1).

functionOr(fos, 1).
functionAnd(fos, 1, erk).

edge(cd45, fyn, 1).
edge(lck, fyn, 1).
edge(tcrbind, fyn, 1).

functionOr(fyn, 1..2).
functionAnd(fyn, 1, cd45).
functionAnd(fyn, 1, lck).
functionAnd(fyn, 2, cd45).
functionAnd(fyn, 2, tcrbind).

edge(lat, gads, 1).

functionOr(gads, 1).
functionAnd(gads, 1, lat).

edge(lat, grb2sos, 1).

functionOr(grb2sos, 1).
functionAnd(grb2sos, 1, lat).

edge(pkcth, ikk, 1).

functionOr(ikk, 1).
functionAnd(ikk, 1, pkcth).

edge(plcg_a, ip3, 1).

functionOr(ip3, 1).
functionAnd(ip3, 1, plcg_a).

edge(ikk, ikb, 0).

functionOr(ikb, 1).
functionAnd(ikb, 1, ikk).

edge(zap70, itk, 1).
edge(slp76, itk, 1).

functionOr(itk, 1).
functionAnd(itk, 1, zap70).
functionAnd(itk, 1, slp76).

edge(sek, jnk, 1).

functionOr(jnk, 1).
functionAnd(jnk, 1, sek).

edge(jnk, jun, 1).

functionOr(jun, 1).
functionAnd(jun, 1, jnk).

edge(zap70, lat, 1).

functionOr(lat, 1).
functionAnd(lat, 1, zap70).

edge(cd45, lck, 1).
edge(cd8, lck, 1).
edge(pagcsk, lck, 0).

functionOr(lck, 1).
functionAnd(lck, 1, cd45).
functionAnd(lck, 1, cd8).
functionAnd(lck, 1, pagcsk).

edge(raf, mek, 1).

functionOr(mek, 1).
functionAnd(mek, 1, raf).

edge(calcin, nfat, 1).

functionOr(nfat, 1).
functionAnd(nfat, 1, calcin).

edge(ikb, nfkb, 0).

functionOr(nfkb, 1).
functionAnd(nfkb, 1, ikb).

edge(tcrbind, pagcsk, 0).

functionOr(pagcsk, 1).
functionAnd(pagcsk, 1, tcrbind).

edge(dag, pkcth, 1).

functionOr(pkcth, 1).
functionAnd(pkcth, 1, dag).

edge(zap70, plcg_a, 1).
edge(itk, plcg_a, 1).
edge(slp76, plcg_a, 1).
edge(plcg_b, plcg_a, 1).
edge(rlk, plcg_a, 1).

functionOr(plcg_a, 1..2).
functionAnd(plcg_a, 1, zap70).
functionAnd(plcg_a, 1, itk).
functionAnd(plcg_a, 1, slp76).
functionAnd(plcg_a, 1, plcg_b).
functionAnd(plcg_a, 2, rlk).
functionAnd(plcg_a, 2, zap70).
functionAnd(plcg_a, 2, slp76).
functionAnd(plcg_a, 2, plcg_b).

edge(lat, plcg_b, 1).

functionOr(plcg_b, 1).
functionAnd(plcg_b, 1, lat).

edge(ras, raf, 1).

functionOr(raf, 1).
functionAnd(raf, 1, ras).

edge(grb2sos, ras, 1).
edge(rasgrp1, ras, 1).

functionOr(ras, 1).
functionAnd(ras, 1, grb2sos).
functionAnd(ras, 1, rasgrp1).

edge(dag, rasgrp1, 1).
edge(pkcth, rasgrp1, 1).

functionOr(rasgrp1, 1).
functionAnd(rasgrp1, 1, dag).
functionAnd(rasgrp1, 1, pkcth).

edge(lck, rlk, 1).

functionOr(rlk, 1).
functionAnd(rlk, 1, lck).

edge(erk, rsk, 1).

functionOr(rsk, 1).
functionAnd(rsk, 1, erk).

edge(pkcth, sek, 1).

functionOr(sek, 1).
functionAnd(sek, 1, pkcth).

edge(gads, slp76, 1).

functionOr(slp76, 1).
functionAnd(slp76, 1, gads).

edge(tcrlig, tcrbind, 1).
edge(ccbl, tcrbind, 0).

functionOr(tcrbind, 1).
functionAnd(tcrbind, 1, tcrlig).
functionAnd(tcrbind, 1, ccbl).

edge(tcrlig, tcrlig, 1).

functionOr(tcrlig, 1).
functionAnd(tcrlig, 1, tcrlig).

edge(tcrbind, tcrphos, 1).
edge(lck, tcrphos, 1).
edge(fyn, tcrphos, 1).

functionOr(tcrphos, 1..2).
functionAnd(tcrphos, 1, tcrbind).
functionAnd(tcrphos, 1, lck).
functionAnd(tcrphos, 2, fyn).

edge(lck, zap70, 1).
edge(ccbl, zap70, 0).
edge(tcrphos, zap70, 1).

functionOr(zap70, 1).
functionAnd(zap70, 1, lck).
functionAnd(zap70, 1, ccbl).
functionAnd(zap70, 1, tcrphos).

edge(zap70, ccbl, 1).

functionOr(ccbl, 1).
functionAnd(ccbl, 1, zap70).

%exp(p1).