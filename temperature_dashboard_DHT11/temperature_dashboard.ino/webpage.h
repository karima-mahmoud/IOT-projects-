#ifndef WEBPAGE_H
#define WEBPAGE_H

const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Temperature Dashboard</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    *{margin:0;padding:0;box-sizing:border-box;}
    body{
      font-family:Arial,sans-serif;
      background:linear-gradient(135deg,#0b1220,#111827,#1e293b);
      color:white;
      min-height:100vh;
      padding:20px;
    }
    .wrapper{
      max-width:1300px;
      margin:auto;
    }
    .header{
      text-align:center;
      margin-bottom:24px;
    }
    .header h1{
      font-size:clamp(2rem,5vw,3.4rem);
      color:#7dd3fc;
      margin-bottom:10px;
    }
    .header p{
      color:#cbd5e1;
      font-size:clamp(1rem,2vw,1.15rem);
    }
    .top-grid{
      display:grid;
      grid-template-columns:repeat(auto-fit,minmax(240px,1fr));
      gap:18px;
      margin-bottom:22px;
    }
    .card{
      background:rgba(15,23,42,.9);
      border:1px solid rgba(255,255,255,.08);
      border-radius:22px;
      padding:22px;
      box-shadow:0 8px 25px rgba(0,0,0,.25);
    }
    .card h3{
      color:#cbd5e1;
      font-size:1rem;
      margin-bottom:12px;
    }
    .value{
      font-size:clamp(1.8rem,4vw,2.8rem);
      font-weight:bold;
      margin-bottom:8px;
    }
    .sub{
      color:#94a3b8;
      font-size:.95rem;
    }
    .charts-grid{
      display:grid;
      grid-template-columns:repeat(auto-fit,minmax(320px,1fr));
      gap:20px;
      margin-bottom:22px;
    }
    .chart-card{
      background:rgba(15,23,42,.92);
      border:1px solid rgba(255,255,255,.08);
      border-radius:22px;
      padding:20px;
      box-shadow:0 8px 25px rgba(0,0,0,.25);
    }
    .chart-title{
      font-size:1.1rem;
      margin-bottom:14px;
      color:#7dd3fc;
    }
    canvas{
      width:100% !important;
      height:320px !important;
    }
    .analysis-grid{
      display:grid;
      grid-template-columns:repeat(auto-fit,minmax(220px,1fr));
      gap:18px;
    }
    .mini{
      background:rgba(30,41,59,.92);
      border:1px solid rgba(255,255,255,.06);
      border-radius:18px;
      padding:18px;
    }
    .mini .label{
      color:#94a3b8;
      margin-bottom:8px;
      font-size:.95rem;
    }
    .mini .big{
      font-size:1.5rem;
      font-weight:bold;
    }
    .badge{
      display:inline-block;
      padding:8px 14px;
      border-radius:999px;
      font-size:1rem;
      font-weight:bold;
      margin-top:6px;
      background:#334155;
    }
    .cool{ background:#2563eb; }
    .normal{ background:#16a34a; }
    .hot{ background:#dc2626; }

    @media (max-width:700px){
      body{padding:12px;}
      canvas{height:260px !important;}
    }
  </style>
</head>
<body>
  <div class="wrapper">
    <div class="header">
      <h1>Temperature Dashboard</h1>
      <p>Live monitoring and analysis for temperature and humidity</p>
    </div>

    <div class="top-grid">
      <div class="card">
        <h3>Current Temperature</h3>
        <div class="value" id="tempValue">-- °C</div>
        <div class="sub">Real-time sensor reading</div>
      </div>

      <div class="card">
        <h3>Current Humidity</h3>
        <div class="value" id="humValue">-- %</div>
        <div class="sub">Real-time sensor reading</div>
      </div>

      <div class="card">
        <h3>Environment Status</h3>
        <div class="value" id="envText">--</div>
        <div class="badge" id="envBadge">Status</div>
      </div>
    </div>

    <div class="charts-grid">
      <div class="chart-card">
        <div class="chart-title">Temperature History</div>
        <canvas id="tempChart"></canvas>
      </div>

      <div class="chart-card">
        <div class="chart-title">Humidity History</div>
        <canvas id="humChart"></canvas>
      </div>
    </div>

    <div class="analysis-grid">
      <div class="mini">
        <div class="label">Maximum Temperature</div>
        <div class="big" id="maxTemp">-- °C</div>
      </div>

      <div class="mini">
        <div class="label">Minimum Temperature</div>
        <div class="big" id="minTemp">-- °C</div>
      </div>

      <div class="mini">
        <div class="label">Average Temperature</div>
        <div class="big" id="avgTemp">-- °C</div>
      </div>

      <div class="mini">
        <div class="label">Average Humidity</div>
        <div class="big" id="avgHum">-- %</div>
      </div>

      <div class="mini">
        <div class="label">Last Update</div>
        <div class="big" id="lastUpdate">--</div>
      </div>
    </div>
  </div>

  <script>
    let tempChart;
    let humChart;

    function makeChart(canvasId, label, borderColor, backgroundColor){
      const ctx = document.getElementById(canvasId).getContext('2d');
      return new Chart(ctx, {
        type: 'line',
        data: {
          labels: [],
          datasets: [{
            label: label,
            data: [],
            borderColor: borderColor,
            backgroundColor: backgroundColor,
            tension: 0.35,
            fill: true
          }]
        },
        options: {
          responsive: true,
          maintainAspectRatio: false,
          plugins: {
            legend: { labels: { color: '#fff' } }
          },
          scales: {
            x: {
              ticks: { color: '#cbd5e1' },
              grid: { color: 'rgba(255,255,255,0.08)' }
            },
            y: {
              ticks: { color: '#cbd5e1' },
              grid: { color: 'rgba(255,255,255,0.08)' }
            }
          }
        }
      });
    }

    function calcAvg(arr){
      if(!arr.length) return 0;
      return arr.reduce((a,b)=>a+b,0)/arr.length;
    }

    function setEnvironment(status){
      const envText = document.getElementById('envText');
      const envBadge = document.getElementById('envBadge');

      envText.textContent = status;
      envBadge.textContent = status;
      envBadge.className = 'badge';

      if(status === 'Cool') envBadge.classList.add('cool');
      else if(status === 'Normal') envBadge.classList.add('normal');
      else if(status === 'Hot') envBadge.classList.add('hot');
    }

    async function fetchData(){
      const res = await fetch('/data');
      const data = await res.json();

      document.getElementById('tempValue').textContent = data.temperature.toFixed(1) + ' °C';
      document.getElementById('humValue').textContent = data.humidity.toFixed(1) + ' %';
      setEnvironment(data.environment);
      document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
    }

    async function fetchHistory(){
      const res = await fetch('/history');
      const data = await res.json();

      tempChart.data.labels = data.labels;
      tempChart.data.datasets[0].data = data.temps;
      tempChart.update();

      humChart.data.labels = data.labels;
      humChart.data.datasets[0].data = data.hums;
      humChart.update();

      if(data.temps.length){
        document.getElementById('maxTemp').textContent = Math.max(...data.temps).toFixed(1) + ' °C';
        document.getElementById('minTemp').textContent = Math.min(...data.temps).toFixed(1) + ' °C';
        document.getElementById('avgTemp').textContent = calcAvg(data.temps).toFixed(1) + ' °C';
      }

      if(data.hums.length){
        document.getElementById('avgHum').textContent = calcAvg(data.hums).toFixed(1) + ' %';
      }
    }

    async function refreshAll(){
      await fetchData();
      await fetchHistory();
    }

    window.onload = () => {
      tempChart = makeChart('tempChart', 'Temperature (°C)', '#38bdf8', 'rgba(56,189,248,0.12)');
      humChart = makeChart('humChart', 'Humidity (%)', '#f472b6', 'rgba(244,114,182,0.10)');
      refreshAll();
      setInterval(refreshAll, 3000);
    };
  </script>
</body>
</html>
)rawliteral";

#endif