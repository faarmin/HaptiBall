R"=====(
<!DOCTYPE HTML>
<html>

<head>
    <title>Testing Suite</title>>
    <link rel="stylesheet" href="styles.css">
     <!--  <script src="threejs.js"></script> -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
</head>

<body>
    <div class="banner">
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
                <button id="reset">RESET POSITION </button>

            </div>
              <div class="card">
                  <p class="card-title">CUSTOM HIT</p>
                  ACTIVATE <input type="checkbox" id="configureHit"></br></br>
                  Repetitions:<input type="number" id="reps" length="2" value="3" /> </br></br>
                  Active time:<input type="number" id="actTime" length="3" value="30" /> </br></br>
                  Time between hits:<input type="number" id="waitTime" length="2" value="50" /> </br>
            </div>
            <div class="card">
                <p class="card-title">IMPACT VECTOR</p>
                <div id="vectorHit">
                  X<input type="range" id="hitX" min="-10" max="10" value="5" /><span class="fixed-width-span" id="numHitX">5</span></br>
                  Y<input type="range" id="hitY" min="-10" max="10" value="5" /><span class="fixed-width-span" id="numHitY">5</span></br>
                  Z<input type="range" id="hitZ" min="-10" max="10" value="5" /><span class="fixed-width-span" id="numHitZ">5</span></br></br>
                  <button id="hitBtn">SIMULATE HIT</button></br></br>
                  <button id="launch">LAUNCH MOTORS</button></br></br>
                  <button id="hideVectorsBtn">CLEAN</button></br>
                  

                </div>
            </div>
             <div class="cardSliders">
                <p class="card-title">ACTIVATE</p>
                
                </br>
                <div class="button-slider-pair">
                    <button class="base-btn" id="Sup">MOTOR SUPERIOR </button> 
                    <input type="range" id="slider1" min="0" max="10" value="10" /> <span class="fixed-width-span" id="numPower1">10</span> power
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mNorte">MOTOR NORTE </button>
                    <input type="range" id="slider2" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower2">10</span> power
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mSur" >MOTOR SUR</button>
                    <input type="range" id="slider3" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower3">10</span> power</br>
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mEste" >MOTOR ESTE</button>
                    <input type="range" id="slider4" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower4">10</span> power</br>
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="mOeste">MOTOR OESTE</button>
                    <input type="range" id="slider5" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower5">10</span> power</br>
                </div>
                <div class="button-slider-pair">
                    <button class="base-btn" id="Inf" >MOTOR INFERIOR </button>
                    <input type="range" id="slider6" min="0" max="10" value="10" /><span class="fixed-width-span" id="numPower6">10</span> power</br>
                </div>

            </div>

    </div>
</body>

<script type="importmap">
    {
        "imports": {
          "three": "https://unpkg.com/three@0.155.0/build/three.module.js",
          "three/addons/": "https://unpkg.com/three@0.155.0/examples/jsm/",
          "dat.gui": "https://unpkg.com/dat.gui@0.7.9/build/dat.gui.module.js"
        }
    }
    </script>

<script  type="module" defer  src="script.js"></script>

</html>

)=====";