# Model Revision using ASP

The input files must contain the description of the model, the regulatory functions, and the experimental data.

#### Input Format

*The definition of input should follow the order presented in this document. Different definition order may result in unexpected behaviour.*

The model is represented by a list of edges, each with a sign associated.
```
edge(<from>,<to>,<sign>).
```
Where `<from>` is the origin of the edge with **lowercase letters or numbers**, `<to>` is the destination, and `<sign>` is '0' for a negative edge or '1' otherwise.
For example, if we want to express a negative edge from *c1* to *c2*:
```
edge(c1,c2,0).
```

The model may also contain the definition of vertices.
```
vertex(<compound>).
```
Where `<compound>` is represented in **lowercase letters or numbers**.
For example if we want to define explicitly the existence of compound *c3*, we define:
```
vertex(c3).
```
Note that all vertices are inferred from the defined edges, therefore its definitions are not necessary. However, if there is a vertex without incoming or outgoing edges, its explicit definition must be made by the predicate `vertex`.


The regulatory functions of each compound must be represented in DNF form.
```
functionOr(<compound>,1..<end>).
functionAnd(<compound>,<clauseID>,<regulatorCompound>).
```
Where `<compound>` represents the regulated compound (in lowercase letters or numbers), `<end>` represents the number of clauses in the DNF, `<clauseID>` identifies the each of the clauses, and `<regulatorCompound>` represents the regulator present in the clause (in lowercase letters or numbers).
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

##### Extra

To prevent some repairs to be considered, we support the definition of two additional predicates.
To prevent repair operations that change or remove a given edge, we use the following predicate:
```
fixed(<from>,<to>).
```
Where `<from>` is the origin of the edge with **lowercase letters or numbers**, and `<to>` is the destination.
For example, if we want to prevent changes on an edge from *c1* to *c2*, we define:
```
fixed(c1,c2).
```

To prevent repair operations that change a given node (changing the regulatory function, changing the sign of the incoming edges, removing incoming edges, or adding new incoming edges), we use the following predicate:
```
fixed(<compound>).
```
Where `<compound>` is the node that we do not want to change (in lowercase letters or numbers).
For example, if we want to prevent changes on node *c1*, we define:
```
fixed(c1).
```

**! Note:** All values must start with a number or a lowercase letter. All unexpected values may produce wrong results, or produce errors.
**! Note:** Unexpected predicates are ignored.