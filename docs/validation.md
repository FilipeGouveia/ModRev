<!--Here are the results obtained with ModRev v1.2-->
## Instances

#### Models
Five well known biological models:
 - **FY**  : Fission Yeast by Davidich and Bornholdt (2008)
 - **SP**  : Segment Polarity (1 cell) by Sánchez *et al.* (2002)
 - **TCR** : TCR Signalisation by Klamt *et al.* (2006)
 - **MCC** : Mammalian Cell Cycle by Fauré *et al.* (2006)
 - **Th**  : T-helper Cell Differentiation by Mendoza and Xenarios (2006)

Models were corrupted according to different parameters:
 - **F** : probability of changing a regulatory function
 - **E** : probability of flipping the sign of an edge
 - **R** : probability of removing a regulator
 - **A** : probability of adding a regulator

24 different configurations of corruption parameters were considered and 100 model instances were generated for each configuration.

Generated models can be found [here](https://filipegouveia.github.io/ModelRevisionASP/inputNetworks.zip).

#### Observations

Five sets of observations were generated consistent with the original models under the following update schemes:
 - Synchronous
 - Asynchronous

The observations were generated with a length of 20 time-steps. For the experimental evaluation it were considered also a part of the same observations with only 3 time-steps.

Generated observations can be found [here](https://filipegouveia.github.io/ModelRevisionASP/observations.zip).


## Results

<!-- A table summary of the time results can be found here -->

<div>
    <div style="width:50%">Synchronous</div>
    <div style="width:50%">Asynchronous</div>
</div>

<div>
    <div style="width:50%">
        ![FY - Synchronous](images/FY-s.png)
    </div>
    <div style="width:50%">
        ![FY - Asynchronous](images/FY-a.png)
    </div>
</div>
