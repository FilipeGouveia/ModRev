# Model Revision of Boolean Logical Models of Biological Regulatory Networks

Model revision tool to repair inconsistent logical models of biological regulatory networks.

## Getting Started

These instructions will let you compile and use our tool for tests purposes.

### Dependencies

This tool has a dependency of C++ boost libraries, available online. ([boost.org](https://boost.org))

Make sure that boost libraries are installed before compiling the tool.



This tool also depends on [clingo](https://potassco.org/clingo/) libraries that are available in src/libUNIX and src/libOSX.

However, you can always download clingo packages, compile the libraries, and replace the corresponding libraries in ModRev/src/libUNIX or ModRev/src/libOSX.
To compile clingo libraries, extract the package and inside the extracted folder do:
```
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release -DCLINGO_BUILD_STATIC=ON
cmake --build build
```

The clingo libraries should then be found in build/lib.

ModRev also depends on [BooleanFunction](https://github.com/FilipeGouveia/BooleanFunction) library.
If you need to recompile and replace the corresponding library, download the library, follow the instruction to compile and replace the library in ModRev/src/libUNIX or ModRev/src/libOSX.

### Compiling

This tool is only available for macOS or Linux.

To compile do:
```
make
```

Use
```
make config
```
to define the initial configuration parameters.


## Usage

### Model Revision Tool

Examples of inconsistent networks are provided in [inputNetworks](https://filipegouveia.github.io/ModRev/inputNetworks.zip).

Examples of time-series observations are provided in [observations](https://filipegouveia.github.io/ModRev/observations.zip).

To use this tool, do:

```
./modrev [-m] <input network> [[-obs] <observation file 1> ...] [options]
```

Check `./modrev --help` for more details of the available options.

Example of usage under stable state observations:
```
./modrev -m examples/model.lp -obs examples/obsSS.lp -ss
```

Example of usage under stable state observations with a fixed edge:
```
./modrev -m examples/modelFixed.lp -obs examples/obsSS.lp -ss
```

Examples of usage under time-series observations with synchronous update:
```
./modrev -m examples/model.lp -obs obsTS01.lp -up s
```
```
./modrev -m examples/model.lp -obs obsTS02.lp -up s
```


##### Other examples

Example of usage under stable state observations:
```
./modrev -m inputNetworks/boolean_cell_cycle/00/boolean_cell_cycle-50-0-0-0-net-att.lp -ss
```

Example of usage under time-series observations with asynchrounous update:
```
./modrev -m inputNetworks/boolean_cell_cycle/00/boolean_cell_cycle-50-0-0-0-net.lp -obs observations/boolean_cell_cycle/observations/async/a_o5_t3.lp -up a
```


### Output

The ModRev tool produces as output the information regarding the minimal repairs that can be performed to render an inconsistent model consistent.

If the model is consistent with the given observations, the follwing message is displayed:
```
This network is consistent!
```

In case of inconsistency, a solution will be displayed after the following message:
```
### Found solution with <N> repair operations.
```
where `<N>` indicates the minimal number of repair operations.
If more solutions are found, this message will be displayed before each solution.

Each solution presents the repair operations that can be applied for each node considered inconsistent.
A message will be displayed indicating the node affected by the operations:
```
Inconsistent node <V>.
```
where `<V>` is the node.

To repair an inconsistent node multiple sets of operations can be applied.
Each set of repair operations are introduced with the message:
```
Repair #<i>:
```
where `<i>` is a natural number that will be incremented for each possible set of repair operations.
Note that the sets of repair operations are mutually exclusive. In order to repair an inconsistent node **only one** set of repairs should be aplied.

Example of an output with three inconsistent nodes:
```
### Found solution with 4 repair operations.
	Inconsistent node v1.
		Repair #1:
			Flip sign of edge (v2,v1).
	Inconsistent node v2.
		Repair #1:
			Change function of v2 to (v1) || (v4)
	Inconsistent node v3.
		Repair #1:
			Change function of v3 to (v1 && v2) || (v1 && v4)
			Flip sign of edge (v1,v3).
		Repair #2:
			Change function of v3 to (v1 && v2) || (v2 && v4)
			Flip sign of edge (v2,v3).
```
Note that in order to repair node *v3*, there are two possiblities, and exactly one of them must be chosen (Repair #1 or Repair #2).

A machine style, easily parsable output format is supported (check `./modrev --help`).
In this minimalistic format:
 - Each line represents a different solution;
 - Symbol "/" separates each inconsistent node;
 - Symbol "@" separates the inconsistent node from the corresponding sets of repair operations;
 - Symbol ";" separates different sets of repair operations;
 - Symbol ":" separates each repair operation.
 - Each repair operation has the following format:
    - A,`<V1>`,`<V2>`,`<S>`     indicating the **A**ddition of an edge from `<V1>` to `<V2>` with sign `<S>`;
    - R,`<V1>`,`<V2>`           indicating the **R**emoval of the edge from `<V1>` to `<V2>`;
    - E,`<V1>`,`<V2>`           indicating the flip of the sign of the **E**dge from `<V1>` to `<V2>`;
    - F,`<Function>`              indicating the change of the regulatory **F**unction to `<Function>`.

The previous example can be represented as:
```
v1@E,v2,v1/v2@F,(v1) || (v4)/v3@E,v1,v3:F,(v1 && v2) || (v1 && v4);E,v2,v3:F,(v1 && v2) || (v2 && v4)
```

Sub-optimal solutions in machine style output are preceeded by a "+" symbol.

**Note:** The format of the input files are described in more detail in *ASP/README.md*.

## Authors
* Filipe Gouveia
* Inês Lynce
* Pedro Monteiro