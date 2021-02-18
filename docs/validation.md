[<< Back](/ModelRevisionASP)
<!--Here are the results obtained with ModRev v1.2-->
## Instances

#### Models
Five well known biological models:
 - **FY**  : Fission Yeast by Davidich and Bornholdt (2008) ([doi:10.1371/journal.pone.0001672](https://doi.org/10.1371/journal.pone.0001672))
 - **SP**  : Segment Polarity (1 cell) by Sánchez *et al.* (2002) ([doi:10.1387/ijdb.072439ls](https://doi.org/10.1387/ijdb.072439ls))
 - **TCR** : TCR Signalisation by Klamt *et al.* (2006) ([doi:10.1186/1471-2105-7-56](https://doi.org/10.1186/1471-2105-7-56))
 - **MCC** : Mammalian Cell Cycle by Fauré *et al.* (2006) ([doi:10.1093/bioinformatics/btl210](https://doi.org/10.1093/bioinformatics/btl210))
 - **Th**  : T-helper Cell Differentiation by Mendoza and Xenarios (2006) ([doi:10.1186/1742-4682-3-13](https://doi.org/10.1186/1742-4682-3-13))

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

For detailed time results, please check [results-global-v1.2.3.pdf](/ModelRevisionASP/results-global-v1.2.3.pdf).
<!-- For detailed time results, please check [results-v1.2.pdf](/ModelRevisionASP/results-v1.2.pdf). -->
<!-- A table summary of the time results can be found here -->
<!--For a comparative view, please check [here](compare)-->


obs - number of observations

ts  - number of time-steps

<div class="inner float small-no-side-pad">
    <div class="big-50 small-100 text-center">
        <h4>Synchronous</h4>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/FY-s.png" alt="FY - Synchronous"/>
        FY - Synchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/SP-s.png" alt="SP - Synchronous"/>
        SP - Synchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/TCR-s.png" alt="TCR - Synchronous"/>
        TCR - Synchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/MCC-s.png" alt="MCC - Synchronous"/>
        MCC - Synchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/th-s.png" alt="Th - Synchronous"/>
        Th - Synchronous
        </div>
    </div>
    <div class="big-50 small-100 text-center">
        <h4>Asynchronous</h4>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/FY-a.png" alt="FY - Asynchronous"/>
        FY - Asynchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/SP-a.png" alt="SP - Asynchronous"/>
        SP - Asynchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/TCR-a.png" alt="TCR - Asynchronous"/>
        TCR - Asynchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/MCC-a.png" alt="MCC - Asynchronous"/>
        MCC - Asynchronous
        </div>
        <div class="inner small-no-side-pad small-margin-right">
        <img src="images/th-a.png" alt="Th - Asynchronous"/>
        Th - Asynchronous
        </div>
    </div>
</div>

<!--
![FY - Synchronous](images/FY-s.png)

<p style="text-align:center;">FY - Synchronous</p>

![FY - Asynchronous](images/FY-a.png)

<p style="text-align:center;">FY - Asynchronous</p>

![SP - Synchronous](images/SP-s.png)

<p style="text-align:center;">SP - Synchronous</p>

![SP - Asynchronous](images/SP-a.png)

<p style="text-align:center;">SP - Asynchronous</p>

![TCR - Synchronous](images/TCR-s.png)

<p style="text-align:center;">TCR - Synchronous</p>

![TCR - Asynchronous](images/TCR-a.png)

<p style="text-align:center;">TCR - Asynchronous</p>

![MCC - Synchronous](images/MCC-s.png)

<p style="text-align:center;">MCC - Synchronous</p>

![MCC - Asynchronous](images/MCC-a.png)

<p style="text-align:center;">MCC - Asynchronous</p>

![Th - Synchronous](images/th-s.png)

<p style="text-align:center;">Th - Synchronous</p>

![FY - Asynchronous](images/th-a.png)

<p style="text-align:center;">Th - Asynchronous</p>
-->
<!--
<div class="inner" style="display:flex">
    <div class="inner" style="width:50%;float:left;text-align:center">
        <h6>Synchronous</h6>
        <div class="inner">
        <img style="max-width:100%" src="images/FY-s.png" alt="FY - Synchronous"/>
        FY
        </div>
    </div>
    <div class="inner" style="width:50%;float:right;text-align:center">
        <h6>Asynchronous</h6>
        <div class="inner">
        <img style="max-width:100%" src="images/FY-a.png" alt="FY - Asynchronous"/>
        FY
        </div>
    </div>
</div>
-->
<!--
<div>
    <div style="width:50%">
        ![FY - Synchronous](images/FY-s.png)
    </div>
    <div style="width:50%">
        ![FY - Asynchronous](images/FY-a.png)
    </div>
</div>
-->