# Model Revision using ASP

The input files must contain the description of the model, the regulatory functions, and the experimental data.

#### Input Format

The model is represented by a list of edges, each with a sign associated.
```
edge(<from>,<to>,<sign>).
```
Where `<from>` is the origin of the edge with small letters or numbers, `<to>` is the destination, and `<sign>` is '0' for a negative edge or '1' otherwise.
For example, if we want to express a negative edge from *c1* to *c2*:
```
edge(c1,c2,0).
```

The regulatory functions of each compound must be represented in DNF form.
```
functionOr(<compound>,1..<end>).
functionAnd(<compound>,<clauseID>,<regulatorCompound>).
```
where `<compound>` represents the regulated compound, `<end>` represents the number of clauses in the DNF, `<clauseID>` identifies the each of the clauses, and `<regulatorCompound>` represents the regulator present in the clause.
Each regulatory function has one **functionOr** predicate and one or more **functionAnd** predicate.
For example, if compound *c3* has the regulatory function *((c1) or (c2 and c4))*, wich has two clauses, we define:
```
functionOr(c3,1..2).
functionAnd(c3,1,c1).
functionAnd(c3,2,c2).
functionAnd(c3,2,c4).
```

##### Observations

Multiple observations can be provided.
To identify each experimental profile, use the follwing predicate:
```
exp(<profile>).
```

###### Stable State Observations

The observed values are represented with the following predicate:
```
obs_vlabel(<profile>,<compound>,<sign>).
```
For example, if the compound *c1* was observed with a positive sign in experiment profile *p*, we define:
```
obs_vlabel(p,c1,1).
```

###### Time-series Observations

The observed values are represented with the following predicate:
```
obs_vlabel(<profile>,<time>,<compound>,<sign>).
```
For example, if the compound *c1* was observed with a positive sign in experiment profile *p* in time step *2*, we define:
```
obs_vlabel(p,2,c1,1).
```
To define the maximum time-steps to be considered, the following line must be added:
```
#const t = <Time Limit>.
```
