edge(cycb, cdc20, 1).

functionOr(cdc20, 1).
functionAnd(cdc20, 1, cycb).

edge(rb, cyca, 0).
edge(cyca, cyca, 1).
edge(cdc20, cyca, 0).
edge(cdh1, cyca, 0).
edge(ubch10, cyca, 0).
edge(e2f, cyca, 1).

functionOr(cyca, 1..4).
functionAnd(cyca, 1, rb).
functionAnd(cyca, 1, cyca).
functionAnd(cyca, 1, cdc20).
functionAnd(cyca, 1, cdh1).
functionAnd(cyca, 2, rb).
functionAnd(cyca, 2, cyca).
functionAnd(cyca, 2, cdc20).
functionAnd(cyca, 2, ubch10).
functionAnd(cyca, 3, rb).
functionAnd(cyca, 3, e2f).
functionAnd(cyca, 3, cdc20).
functionAnd(cyca, 3, cdh1).
functionAnd(cyca, 4, rb).
functionAnd(cyca, 4, e2f).
functionAnd(cyca, 4, cdc20).
functionAnd(cyca, 4, ubch10).

edge(cdc20, cycb, 0).
edge(cdh1, cycb, 0).

functionOr(cycb, 1).
functionAnd(cycb, 1, cdc20).
functionAnd(cycb, 1, cdh1).

edge(cycd, cycd, 1).

functionOr(cycd, 1).
functionAnd(cycd, 1, cycd).

edge(rb, cyce, 0).
edge(e2f, cyce, 1).

functionOr(cyce, 1).
functionAnd(cyce, 1, rb).
functionAnd(cyce, 1, e2f).

edge(rb, e2f, 0).
edge(cyca, e2f, 0).
edge(cycb, e2f, 0).
edge(p27, e2f, 1).

functionOr(e2f, 1..2).
functionAnd(e2f, 1, rb).
functionAnd(e2f, 1, cyca).
functionAnd(e2f, 1, cycb).
functionAnd(e2f, 2, rb).
functionAnd(e2f, 2, p27).
functionAnd(e2f, 2, cycb).

edge(cycd, rb, 0).
edge(cyce, rb, 0).
edge(cyca, rb, 0).
edge(cycb, rb, 0).
edge(p27, rb, 1).

functionOr(rb, 1..2).
functionAnd(rb, 1, cycd).
functionAnd(rb, 1, cyce).
functionAnd(rb, 1, cyca).
functionAnd(rb, 1, cycb).
functionAnd(rb, 2, cycd).
functionAnd(rb, 2, p27).
functionAnd(rb, 2, cycb).

edge(cdc20, ubch10, 1).
edge(ubch10, ubch10, 1).
edge(cycb, ubch10, 1).
edge(cyca, ubch10, 1).
edge(cdh1, ubch10, 0).

functionOr(ubch10, 1..4).
functionAnd(ubch10, 1, cdc20).
functionAnd(ubch10, 1, ubch10).
functionAnd(ubch10, 2, cycb).
functionAnd(ubch10, 2, ubch10).
functionAnd(ubch10, 3, cyca).
functionAnd(ubch10, 3, ubch10).
functionAnd(ubch10, 4, cdh1).

edge(cyca, cdh1, 0).
edge(cycb, cdh1, 0).
edge(p27, cdh1, 1).
edge(cdc20, cdh1, 1).

functionOr(cdh1, 1..3).
functionAnd(cdh1, 1, cyca).
functionAnd(cdh1, 1, cycb).
functionAnd(cdh1, 2, cycb).
functionAnd(cdh1, 2, p27).
functionAnd(cdh1, 3, cdc20).

edge(cycd, p27, 0).
edge(cyce, p27, 0).
edge(cyca, p27, 0).
edge(cycb, p27, 0).
edge(p27, p27, 1).

functionOr(p27, 1..3).
functionAnd(p27, 1, cycd).
functionAnd(p27, 1, cyce).
functionAnd(p27, 1, cyca).
functionAnd(p27, 1, cycb).
functionAnd(p27, 2, cycd).
functionAnd(p27, 2, cyca).
functionAnd(p27, 2, p27).
functionAnd(p27, 2, cycb).
functionAnd(p27, 3, cycd).
functionAnd(p27, 3, cyce).
functionAnd(p27, 3, p27).
functionAnd(p27, 3, cycb).

