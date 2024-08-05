STRINGIFY(
<!DOCTYPE HTML>
<html>

<head>
    <title>Testing Suite</title>>
    <link rel="stylesheet" href="styles.css">
     <!--  <script src="threejs.js"></script> -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
</head>

<body>
    <div class="topnav">
        <h1>Hapti-Ball Testing Suite</h1>
        <input type="checkbox" id="myCheckbox">
        <label for="myCheckbox">Fibonacci Lattice Distribution</label></br>
        <input type="checkbox" id="myCheckbox2">
        <label for="myCheckbox2">Regular Distribution</label></br>
    </div>
    <div class="content">
            <div class="cube-content">
            <div id="3Dcube"></div>
        </div></BR></BR></BR></BR>
        <div class="cards">
            <div class="card">
                <p class="card-title">GYRO</p>
                <p><span class="reading">X: <span id="gyroX"></span> rad</span></p>
                <p><span class="reading">Y: <span id="gyroY"></span> rad</span></p>
                <p><span class="reading">Z: <span id="gyroZ"></span> rad</span></p>
                <p class="card-title">ACCELEROMETER</p>
                <p><span class="reading">X: <span id="accX"></span> ms<sup>2</sup></span></p>
                <p><span class="reading">Y: <span id="accY"></span> ms<sup>2</sup></span></p>
                <p><span class="reading">Z: <span id="accZ"></span> ms<sup>2</sup></span></p>
            </div>
            <div class="card">
                <p class="card-title">ANGULAR ROTATION</p>
                <p>
                    <span class="reading">X: <span id="angX"></span> grados </span>
                    <span class="reading">-><span id="angXrad"></span> radianes</span>
                </p>
                <p>
                    <span class="reading">Y: <span id="angY"></span> grados </sup></span>
                    <span class="reading">-><span id="angYrad"></span> radianes</sup></span>
                </p>
                  </BR></BR>
                <button id="reset" onclick="resetPosition(this)">RESET POSITION</button>

            </div>
              <div class="card">
                  <p class="card-title">PERIODICITY</p>
                  LOOP <input type="checkbox" id="loopCheck"></br></br>
                  Sleep time:<input type="number" id="sleep" length="2" value="3" /> </br>
            </div>
            <div class="card">
                <p class="card-title">IMPACT VECTOR</p>
                <div id="vectorHit">
                  X<input type="range" id="hitX" min="-10" max="10" value="5" /><span class="fixed-width-span" id="numHitX">5</span></br>
                  Y<input type="range" id="hitY" min="-10" max="10" value="5" /><span class="fixed-width-span" id="numHitY">5</span></br>
                  Z<input type="range" id="hitZ" min="-10" max="10" value="5" /><span class="fixed-width-span" id="numHitZ">5</span></br></br>

                  <button id="updateHitVector">updateHitVector</button></br></br>
                  <button id="launch" onclick="animateVectorsToNewCoordinates()">LAUNCH</button>

                </div>
            </div>
             <div class="cardSliders">
                <p class="card-title">ACTIVATE</p>
                
                </br>
                <div class="button-slider-pair">
                    <button class="base-btn" id="Sup" onclick="accionarMotorX(Sup)">Motor Superior </button> 
                    <input type="range" id="slider1" min="0" max="10" value="10" /> <span class="fixed-width-span" id="numPower1">10</span> power
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mNorte"  onclick="accionarMotorX(mNorte)">Motor Central 1</button>
                    <input type="range" id="slider2" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower2">10</span> power
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mSur"  onclick="accionarMotorX(mSur)">Motor Central 2</button>
                    <input type="range" id="slider3" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower3">10</span> power</br>
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mEste"  onclick="accionarMotorX(mEste)">Motor Central 3</button>
                    <input type="range" id="slider4" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower4">10</span> power</br>
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mOeste"  onclick="accionarMotorX(mOeste)">Motor Central 4</button>
                    <input type="range" id="slider5" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower5">10</span> power</br>
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="Inf"  onclick="accionarMotorX(Inf)">Motor Inferior</button>
                    <input type="range" id="slider6" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower6">10</span> power</br>
                </div>

            </div>

    </div>

</body>
<script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/tween.js/18.6.4/tween.umd.js"></script>
<script src="script.js"></script>
</html>

)