# Model Revision of Boolean Regulatory Networks in Stable State

Model revision tool to repair inconsistent logical models under stable state.

## Getting Started

These instructions will let you compile and use our tool for tests purposes.

### Dependencies

This tool has a dependency of C++ boost libraries, available online. ([boost.org](https://boost.org))
Make sure that boost libraries are installed before compiling the tool.

### Compiling

This tool is only available for macOS or Linux.

To compile do:
```
make
```

## Usage

### Model Revision Tool

Examples of inconsistent networks are provided in [inputNetworks](https://filipegouveia.github.io/ModelRevisionASP/inputNetworks.zip).
Examples of time-series observations are provided in [observations](https://filipegouveia.github.io/ModelRevisionASP/observations.zip).

To use this tool, do:

```
./modrev [-m] <input network> [[-obs] <observation file 1> ...] [options]
```

Check `./modrev --help` for more details of the available options.

Example of usage under stable state observations:
```
./modrev -m inputNetworks/boolean_cell_cycle/00/boolean_cell_cycle-50-0-0-0-net-att.lp -ss
```

Example of usage under time-series observations with asynchrounous update:
```
./modrev -m inputNetworks/boolean_cell_cycle/00/boolean_cell_cycle-50-0-0-0-net.lp -obs observations/boolean_cell_cycle/observations/async/a_o5_t3.lp -up a
```

The format of the input file is described in more detail in *ASP/README.md*.

## Authors
* Filipe Gouveia
* Inês Lynce
* Pedro Monteiro