# Model Revision using ASP

A model revision procedure developed using Answer Set Programming.
This projects contains:
* a consistency-check ASP program (ConsistencyCheck module)
* an ASP program to get the sons or fathers of a given function in DNF (Functions module)

## Getting Started

### Compiling

You only need to compile an auxiliary program if you want to use the Functions module and the power set is not available.
To do so, follow this instructions:

```
cd Functions
make
```

## Usage

### ConsistencyCheck module

This module checks if a givel model is consistent with a given set of experimental data.
For now, it is only possible to verify the consistency under steady sate.

To use this model, run:

```
./ConsistencyCheckSS.csh <input files>
```

Example:
```
./ConsistencyCheckSS.csh ConsistencyCheck/facts/input01.lp
```

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

The observed values are represented with the following predicate:
```
obs_vlabel(<compound>,<sign>).
```
For example, if the compound *c1* was observed with a positive sign, we define:
```
obs_vlabel(c1,1).
```

### Functions module

This module calculates the fathers or sons of a given function in DNF.

To use this model, run:

```
./Functions.csh <s|f> <powerSet file> <function file>
```
where `<s|f>` represents if we want to calculate the sons (s) or the fathers (f) of the functions, `<powerSet file>` is the file with the power set of *n* elements, and the `<function file>` is the file with the function. The function is represented by a **clause** predicate indicating the number of clause and the element present (see the files in Functions/facts for examples).

Example:
```
./Functions.csh f Functions/facts/03/powerSet03.lp Functions/facts/03/clauses03-01.lp
```

Examples of the power set files and the function files can be seen under Functions/facts.

To generate the power set file, compile the **powersetGen** program in Functions/ using the makefile, and then execute:
```
./powersetGen <n>
```
where `<n>` is the number of elements in the powerset.

