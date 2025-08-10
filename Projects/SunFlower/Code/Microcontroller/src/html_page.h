#pragma once
#include <Arduino.h>

extern const String captivePortalPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MyPlants Wi-Fi Configuration</title>
    <style>
        body {
            font-family: 'Verdana';
            text-align: center;
            background: radial-gradient(circle at center, #DBDFE1 0%, #83959A 300%);
            color: #3E4346;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            max-width:380px;
            background: rgba(115, 135, 121, .15);
            padding: 25px;
            border-radius: 15px;
            backdrop-filter: blur(10px);
            box-shadow: 0px 0px 20px rgba(0, 0, 0, 0.2);
        }
        h1 {
            font-size: 24px;
            margin-bottom: 20px;
            color: #3E4346;
        }
        label {
            display: block;
            text-align: left;
            margin: 5px 0 5px;
            margin-left: -5px;
            font-weight: bold;
        }
        input[type="text"], input[type="password"] {
            color : rgba(223, 242, 228, .8);
            width: 100%;
            padding: 12px;
            margin-bottom: 20px;
            margin-left: -12px;
            border: 0px;
            border-radius: 10px;
            font-size: 16px;
            background-color: #83959A;
            
        }
        input:focus {
            outline: none;
        }
        input::placeholder {
            color : rgba(223, 242, 228, .8);
        }
        button {
            background-color: #3E4346;
            color: #DBDFE1;
            padding: 12px 20px;
            border: none;
            border-radius: 15px;
            font-size: 18px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }
        button:hover {
            background-color: #83959A;
        }
        footer {
            margin-top: 20px;
            font-size: 12px;
            color: rgba(131, 149, 154, .8);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>MyPlants Wi-Fi Configuration</h1>
        <form action="/configure" method="POST">
            <label for="wifiName">Wi-Fi Name</label>
            <input type="text" id="wifiName" name="wifiName" placeholder="Enter Wi-Fi name" required>
            <label for="wifiPassword">Wi-Fi Password</label>
            <input type="password" id="wifiPassword" name="wifiPassword" placeholder="Enter Wi-Fi Password" required>
            <button type="submit">Save Configuration</button>
        </form>
        <footer>
            <p>Designed by teoelectric ❤️</p>
        </footer>
    </div>
</body>
</html>
)rawliteral";

extern const String saveConfirmationPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuration Saved</title>
    <style>
        body {
            font-family: 'Verdana';
            text-align: center;
            background: radial-gradient(circle at center, #DBDFE1 0%, #83959A 300%);
            color: #3E4346;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            max-width: 380x;
            background: rgba(115, 135, 121, .15);
            padding: 30px;
            border-radius: 15px;
            backdrop-filter: blur(10px);
            box-shadow: 0px 0px 20px rgba(0, 0, 0, 0.2);
        }
        h1 {
            font-size: 24px;
            margin-bottom: 25px;
            color: #3E4346;
        }
        button {
            background-color: #3E4346;
            color: #DBDFE1;
            padding: 12px 20px;
            border: none;
            border-radius: 15px;
            font-size: 18px;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }
        button:hover {
            background-color: #83959A;
        }
        footer {
            margin-top: 25px;
            font-size: 12px;
            color: rgba(131, 149, 154, .8);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Configuration Saved!</h1>
        <p>Wi-Fi credentials have been saved.</p>
        <button onclick="if(window.opener){window.close();}else{window.location.href='/'}">Back to configuration</button>
        <footer>
            <p>Designed by teoelectric ❤️</p>
        </footer>
    </div>
</body>
</html>
)rawliteral";