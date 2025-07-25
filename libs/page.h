#pragma once

const char* page = 
"<!DOCTYPE html>"
"<html lang=\"pt-BR\">"
"<head>"
"  <meta charset=\"UTF-8\" />"
"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/>"
"  <title>Estação Meteorológica</title>"
"  <script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>"
"  <style>"
"    body {"
"      font-family: Arial, sans-serif;"
"      margin: 30px;"
"      background-color: #DDD;"
"    }"
""
"    h2 {"
"      text-align: center;"
"    margin-bottom: 35px;"
"    }"
""
"  .container {"
"    display: flex;"
"    flex-direction: row;"
"    flex-wrap: wrap;"
"    justify-content: center;"
"    gap: 20px;"
"  }"
""
"  .range-group {"
"    background-color: #EEE;"
"    padding: 10px;"
"    flex: 1 1 300px;"
"    box-sizing: border-box;"
"    margin-bottom: 10px;"
"    min-width: 500px;"
"    max-width: 570px;"
"  }"
""
"  .range-group label {"
"  text-align: center;"
"    display: block;"
"    margin-bottom: 5px;"
"    font-weight: bold;"
"  }"
""
"  .range-group input[type=\"range\"] {"
"    width: 100%;"
"    margin-bottom: 20px;"
"  }"
"  "
"  .range-group input[type=\"number\"] {"
"    width: 50px;"
"  }"
"  "
"  .fr {"
"    float: right;"
"  }"
""
"  @media (max-width: 768px) {"
"    .container {"
"    flex-direction: column;"
"    flex-wrap: nowrap;"
"    align-items: stretch;"
"    }"
"  }"
"  </style>"
"</head>"
"<body>"
""
"  <h2>Monitoramento de Sensores</h2>"
""
"  <div class=\"container\">"
"    <div class=\"range-group\">"
"      <label for=\"tempRange\">Temperatura - offset: <span id=\"tempVal\">0</span>°C</label>"
"      <input type=\"range\" id=\"tempRange\" min=\"-1\" max=\"1\" step=\"0.1\" value=\"0\" onchange=\"saveParams()\">"
"    <canvas id=\"tempChart\"></canvas>"
"    <hr>"
"    <h4>Limites</h4>"
"               <span>Min: <input type=\"number\" id=\"tempMin\" min=\"0\" max=\"50\" value=\"0\" onchange=\"saveParams()\"/></span>"
"    <span class=\"fr\">Max: <input type=\"number\" id=\"tempMax\" min=\"0\" max=\"50\" value=\"50\" onchange=\"saveParams()\"/></span>"
"    </div>"
"    <div class=\"range-group\">"
"      <label for=\"humRange\">Umidade - offset: <span id=\"humVal\">0</span>%</label>"
"      <input type=\"range\" id=\"humRange\" min=\"-5\" max=\"5\" value=\"0\" onchange=\"saveParams()\">"
"    <canvas id=\"humChart\"></canvas>"
"    <hr>"
"    <h4>Limites</h4>"
"               <span>Min: <input type=\"number\" id=\"humMin\" min=\"0\" max=\"100\" value=\"0\" onchange=\"saveParams()\"/></span>"
"    <span class=\"fr\">Max: <input type=\"number\" id=\"humMax\" min=\"0\" max=\"100\" value=\"100\" onchange=\"saveParams()\"/></span>"
"    </div>"
"    <div class=\"range-group\">"
"      <label for=\"pressRange\">Pressão - offset: <span id=\"pressVal\">0</span>kPa</label>"
"      <input type=\"range\" id=\"pressRange\" min=\"-10\" max=\"10\" value=\"0\" onchange=\"saveParams()\">"
"    <canvas id=\"pressChart\"></canvas>"
"    </div>"
"  </div>"
""
"  <script src=\"index.js\"></script>"
"</body>"
"</html>";