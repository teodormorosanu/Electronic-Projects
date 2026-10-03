#pragma once
#include <Arduino.h>

// --------------------------------------------------------------------------
// WI-FI CONFIGURATION PORTAL
// --------------------------------------------------------------------------

const char *config_portal = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="theme-color" content="#12161c">
    <meta name="color-scheme" content="dark">
    <link rel="icon" href="data:,">
    <title>EA-236 Setup</title>
    <style>
        :root {
            --bg: #12161c;
            --panel: #1b222b;
            --panel-edge: #2b3441;
            --accent: #35b8ff;
            --accent-soft: rgba(53, 184, 255, .18);
            --good: #4fae7c;
            --bad: #d6543c;
            --text: #ece7dc;
            --muted: #8791a0;
            --mono: ui-monospace, SFMono-Regular, Consolas, "Roboto Mono", monospace;
            --sans: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
        }
        * { box-sizing: border-box; }
        body {
            margin: 0;
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 24px;
            background:
                radial-gradient(circle at 50% 0%, #1a212b 0%, var(--bg) 60%),
                repeating-linear-gradient(0deg, rgba(255, 90, 70, .025) 0px, rgba(255, 90, 70, .025) 1px, transparent 1px, transparent 24px),
                repeating-linear-gradient(90deg, rgba(255, 90, 70, .025) 0px, rgba(255, 90, 70, .025) 1px, transparent 1px, transparent 24px);
            font-family: var(--sans);
            color: var(--text);
            -webkit-tap-highlight-color: transparent;
        }
        .plate {
            width: min(400px, 92vw);
            background: var(--panel);
            border: 1px solid var(--panel-edge);
            border-radius: 10px;
            padding: 28px 26px 22px;
            box-shadow: 0 24px 60px rgba(0, 0, 0, .45);
            animation: rise .5s ease both;
        }
        @keyframes rise {
            from { opacity: 0; transform: translateY(12px); }
            to { opacity: 1; transform: translateY(0); }
        }
        .nameplate { display: flex; flex-direction: column; gap: 4px; margin-bottom: 18px; }
        .nameplate-id {
            display: flex;
            align-items: center;
            gap: 9px;
            font-family: var(--mono);
            font-size: 20px;
            letter-spacing: .04em;
            font-weight: 600;
        }
        .nameplate-sub {
            font-family: var(--mono);
            font-size: 11px;
            letter-spacing: .12em;
            text-transform: uppercase;
            color: var(--muted);
            padding-left: 19px;
        }
        .lamp {
            width: 10px;
            height: 10px;
            border-radius: 50%;
            background: var(--accent);
            box-shadow: 0 0 8px 1px var(--accent);
            animation: pulse 2.4s ease-in-out infinite;
            flex-shrink: 0;
        }
        .lamp--connecting { animation: pulse 1s ease-in-out infinite; }
        .lamp--ok { background: var(--good); box-shadow: 0 0 10px 2px var(--good); animation: none; }
        .lamp--err { background: var(--bad); box-shadow: 0 0 8px 2px var(--bad); animation: pulse 1.4s ease-in-out infinite; }
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: .4; }
        }
        .hairline {
            height: 1px;
            margin: 18px 0;
            background: linear-gradient(90deg, transparent, var(--panel-edge) 20%, var(--panel-edge) 80%, transparent);
        }
        h1 { font-family: var(--mono); font-size: 17px; letter-spacing: .02em; margin: 0 0 8px; }
        .lede { font-size: 13.5px; line-height: 1.5; color: var(--muted); margin: 0 0 20px; }
        label {
            display: block;
            font-family: var(--mono);
            font-size: 10.5px;
            letter-spacing: .1em;
            text-transform: uppercase;
            color: var(--muted);
            margin: 0 0 6px;
        }
        input[type="text"], input[type="password"] {
            width: 100%;
            padding: 12px 14px;
            margin-bottom: 16px;
            background: var(--bg);
            border: 1px solid var(--panel-edge);
            border-radius: 6px;
            color: var(--text);
            font-family: var(--sans);
            font-size: 15px;
            transition: border-color .15s, box-shadow .15s;
        }
        input[type="text"]:focus, input[type="password"]:focus {
            outline: none;
            border-color: var(--accent);
            box-shadow: 0 0 0 3px var(--accent-soft);
        }
        input::placeholder { color: var(--muted); opacity: .7; }
        button[type="submit"] {
            width: 100%;
            padding: 13px;
            border: none;
            border-radius: 6px;
            background: var(--accent);
            color: #062736;
            font-family: var(--mono);
            font-size: 13px;
            font-weight: 700;
            letter-spacing: .08em;
            text-transform: uppercase;
            cursor: pointer;
            transition: filter .15s, transform .08s;
        }
        button[type="submit"]:hover:not(:disabled) { filter: brightness(1.08); }
        button[type="submit"]:active:not(:disabled) { transform: translateY(1px); }
        button[type="submit"]:disabled { opacity: .5; cursor: not-allowed; }
        .status-line {
            display: flex;
            align-items: center;
            gap: 8px;
            min-height: 18px;
            margin: 16px 0 0;
            font-size: 13px;
            color: var(--muted);
        }
        .status-dot { width: 7px; height: 7px; border-radius: 50%; background: var(--panel-edge); flex-shrink: 0; }
        .status-dot--connecting { background: var(--accent); animation: pulse 1s ease-in-out infinite; }
        .status-dot--ok { background: var(--good); animation: none; }
        .status-dot--err { background: var(--bad); animation: none; }
        .plate-footer { display: flex; flex-direction: column; align-items: center; gap: 14px; }
        .btn-outline {
            display: block;
            width: 100%;
            text-align: center;
            padding: 11px;
            border: 1px solid var(--panel-edge);
            border-radius: 6px;
            color: var(--muted);
            font-family: var(--mono);
            font-size: 12px;
            letter-spacing: .06em;
            text-transform: uppercase;
            text-decoration: none;
            cursor: pointer;
            background: transparent;
            transition: border-color .15s, color .15s;
        }
        .btn-outline:hover { border-color: var(--accent); color: var(--text); }
        .credit { margin: 0; font-family: var(--mono); font-size: 10.5px; color: var(--muted); opacity: .6; }
        @media (prefers-reduced-motion: reduce) {
            .plate, .lamp, .status-dot { animation: none !important; }
        }
    </style>
</head>
<body>
    <main class="plate">
        <header class="nameplate">
            <div class="nameplate-id">
                <span class="lamp" id="lamp"></span>
                <span>EA-236</span>
            </div>
            <span class="nameplate-sub">Locomotive Control Unit</span>
        </header>

        <div class="hairline"></div>

        <section>
            <h1>Connect to Wi-Fi</h1>
            <p class="lede">Enter your network details so EA-236 can join it and leave setup mode.</p>

            <form id="wifiForm">
                <label for="wifiSSID">Network name (SSID)</label>
                <input type="text" id="wifiSSID" name="wifiSSID" placeholder="Home network" maxlength="39" autocomplete="off" required>

                <label for="wifiPassword">Password</label>
                <input type="password" id="wifiPassword" name="wifiPassword" placeholder="Network password" maxlength="39" autocomplete="off" required>

                <button type="submit" id="submitBtn">Connect</button>
            </form>

            <p class="status-line" id="statusLine">
                <span class="status-dot" id="statusDot"></span>
                <span id="statusText">Waiting for network details</span>
            </p>
        </section>

        <div class="hairline"></div>

        <footer class="plate-footer">
            <button type="button" class="btn-outline" onclick="goToConfirm()">Network Status / View IP</button>
            <a class="btn-outline" href="/ota">Firmware update (OTA)</a>
            <p class="credit">Designed by teoelectric</p>
        </footer>
    </main>

    <script>
        function goToConfirm() {
            fetch('/status')
                .then(function (res) { return res.text(); })
                .then(function (text) {
                    const value = text.trim();
                    const isIp = /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/.test(value);
                    if (isIp) {
                        window.location.href = '/confirm?ip=' + encodeURIComponent(value);
                    } else {
                        // Pop-up in caz ca nu este conectat
                        alert('Device is not currently connected to any Wi-Fi network.');
                    }
                })
                .catch(function () {
                    alert('Could not retrieve network status. Please try again.');
                });
        }

        (function () {
            const form = document.getElementById('wifiForm');
            const submitBtn = document.getElementById('submitBtn');
            const statusText = document.getElementById('statusText');
            const statusDot = document.getElementById('statusDot');
            const lamp = document.getElementById('lamp');

            function setStatus(state, message) {
                statusDot.className = 'status-dot status-dot--' + state;
                lamp.className = 'lamp lamp--' + state;
                statusText.textContent = message;
            }

            form.addEventListener('submit', function (event) {
                event.preventDefault();
                submitBtn.disabled = true;
                setStatus('connecting', 'Saving credentials');

                const body = new URLSearchParams(new FormData(form));

                fetch('/config', { method: 'POST', body: body })
                    .then(function (response) {
                        if (!response.ok) throw new Error('rejected');
                        setStatus('connecting', 'Joining network');
                        pollStatus(0);
                    })
                    .catch(function () {
                        submitBtn.disabled = false;
                        setStatus('err', 'Setup rejected, check the fields above');
                    });
            });

            function pollStatus(attempt) {
                const maxAttempts = 15;

                fetch('/status')
                    .then(function (response) { return response.text(); })
                    .then(function (text) {
                        const value = text.trim();
                        const isIp = /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/.test(value);

                        if (isIp) {
                            setStatus('ok', 'Connected, address ' + value);
                            setTimeout(function () {
                                window.location.href = '/confirm?ip=' + encodeURIComponent(value);
                            }, 700);
                        } else if (attempt >= maxAttempts) {
                            submitBtn.disabled = false;
                            setStatus('err', 'Could not join that network, try again');
                        } else {
                            setTimeout(function () { pollStatus(attempt + 1); }, 1000);
                        }
                    })
                    .catch(function () {
                        if (attempt < maxAttempts) {
                            setTimeout(function () { pollStatus(attempt + 1); }, 1000);
                        }
                    });
            }
        })();
    </script>
</body>
</html>
)rawliteral";

// --------------------------------------------------------------------------
// CONFIGURATION CONFIRMATION PORTAL
// --------------------------------------------------------------------------

const char *confirm_portal = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="theme-color" content="#12161c">
    <meta name="color-scheme" content="dark">
    <link rel="icon" href="data:,">
    <title>EA-236 Configuration Saved</title>
    <style>
        :root {
            --bg: #12161c;
            --panel: #1b222b;
            --panel-edge: #2b3441;
            --good: #4fae7c;
            --muted: #8791a0;
            --text: #ece7dc;
            --mono: ui-monospace, SFMono-Regular, Consolas, "Roboto Mono", monospace;
            --sans: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
        }
        * { box-sizing: border-box; }
        body {
            margin: 0;
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 24px;
            background:
                radial-gradient(circle at 50% 0%, #1a212b 0%, var(--bg) 60%),
                repeating-linear-gradient(0deg, rgba(255, 90, 70, .025) 0px, rgba(255, 90, 70, .025) 1px, transparent 1px, transparent 24px),
                repeating-linear-gradient(90deg, rgba(255, 90, 70, .025) 0px, rgba(255, 90, 70, .025) 1px, transparent 1px, transparent 24px);
            font-family: var(--sans);
            color: var(--text);
        }
        .plate {
            width: min(400px, 92vw);
            background: var(--panel);
            border: 1px solid var(--panel-edge);
            border-radius: 10px;
            padding: 28px 26px 22px;
            box-shadow: 0 24px 60px rgba(0, 0, 0, .45);
            animation: rise .5s ease both;
        }
        @keyframes rise {
            from { opacity: 0; transform: translateY(12px); }
            to { opacity: 1; transform: translateY(0); }
        }
        .nameplate { display: flex; flex-direction: column; gap: 4px; margin-bottom: 18px; }
        .nameplate-id {
            display: flex;
            align-items: center;
            gap: 9px;
            font-family: var(--mono);
            font-size: 20px;
            letter-spacing: .04em;
            font-weight: 600;
        }
        .nameplate-sub {
            font-family: var(--mono);
            font-size: 11px;
            letter-spacing: .12em;
            text-transform: uppercase;
            color: var(--muted);
            padding-left: 19px;
        }
        .lamp {
            width: 10px;
            height: 10px;
            border-radius: 50%;
            background: var(--good);
            box-shadow: 0 0 10px 2px var(--good);
            flex-shrink: 0;
        }
        .hairline {
            height: 1px;
            margin: 18px 0;
            background: linear-gradient(90deg, transparent, var(--panel-edge) 20%, var(--panel-edge) 80%, transparent);
        }
        .center-section { text-align: center; }
        h1 { font-family: var(--mono); font-size: 17px; letter-spacing: .02em; margin: 0 0 8px; }
        .lede { font-size: 13.5px; line-height: 1.5; color: var(--muted); margin: 0 0 16px; }

        /* Stiluri pentru afisare IP si buton Copy */
        .ip-box {
            display: none;
            align-items: center;
            justify-content: space-between;
            background: rgba(0, 0, 0, 0.25);
            border: 1px solid var(--panel-edge);
            border-radius: 6px;
            padding: 8px 12px;
            margin: 0 0 20px;
        }
        .ip-value {
            font-family: var(--mono);
            font-size: 13px;
            letter-spacing: .05em;
            color: var(--text);
            font-weight: 600;
            user-select: all;
        }
        .btn-copy {
            background: transparent;
            border: 1px solid var(--panel-edge);
            border-radius: 4px;
            color: var(--muted);
            font-family: var(--mono);
            font-size: 11px;
            letter-spacing: .05em;
            text-transform: uppercase;
            padding: 5px 9px;
            cursor: pointer;
            display: flex;
            align-items: center;
            gap: 6px;
            transition: all .2s ease;
        }
        .btn-copy:hover {
            border-color: var(--good);
            color: var(--text);
        }
        .btn-copy.copied {
            border-color: var(--good);
            color: var(--good);
            background: rgba(79, 174, 124, 0.1);
        }

        .btn-outline {
            display: block;
            width: 100%;
            text-align: center;
            padding: 11px;
            border: 1px solid var(--panel-edge);
            border-radius: 6px;
            color: var(--muted);
            font-family: var(--mono);
            font-size: 12px;
            letter-spacing: .06em;
            text-transform: uppercase;
            cursor: pointer;
            background: transparent;
            transition: border-color .15s, color .15s;
        }
        .btn-outline:hover { border-color: var(--good); color: var(--text); }
        .plate-footer { display: flex; flex-direction: column; align-items: center; gap: 14px; margin-top: 4px; }
        .credit { margin: 0; font-family: var(--mono); font-size: 10.5px; color: var(--muted); opacity: .6; }
        @media (prefers-reduced-motion: reduce) {
            .plate { animation: none !important; }
        }
    </style>
</head>
<body>
    <main class="plate">
        <header class="nameplate">
            <div class="nameplate-id">
                <span class="lamp"></span>
                <span>EA-236</span>
            </div>
            <span class="nameplate-sub">Locomotive Control Unit</span>
        </header>

        <div class="hairline"></div>

        <section class="center-section">
            <h1>Connected</h1>
            <p class="lede" id="confirmText">EA-236 joined the network and left setup mode.</p>
            
            <!-- Zona de afisare IP si butonul Copy -->
            <div class="ip-box" id="ipContainer">
                <span class="ip-value" id="ipAddress">--</span>
                <button class="btn-copy" id="copyBtn" onclick="copyIp()">
                    <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                        <rect x="9" y="9" width="13" height="13" rx="2" ry="2"></rect>
                        <path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"></path>
                    </svg>
                    <span id="copyBtnText">Copy</span>
                </button>
            </div>

            <button class="btn-outline" onclick="if(window.opener){window.close();}else{window.location.href='/';}">Back to setup</button>
        </section>

        <div class="hairline"></div>

        <footer class="plate-footer">
            <p class="credit">Designed by teoelectric</p>
        </footer>
    </main>

    <script>
        let currentIp = '';

        (function () {
            const ip = new URLSearchParams(window.location.search).get('ip');
            if (ip) {
                currentIp = ip;
                document.getElementById('confirmText').textContent =
                    'EA-236 joined the network and is reachable at:';
                document.getElementById('ipAddress').textContent = ip;
                document.getElementById('ipContainer').style.display = 'flex';
            }
        })();

        function copyIp() {
            if (!currentIp) return;

            const btn = document.getElementById('copyBtn');
            const btnText = document.getElementById('copyBtnText');

            const onCopied = () => {
                btnText.textContent = 'Copied!';
                btn.classList.add('copied');
                setTimeout(() => {
                    btnText.textContent = 'Copy';
                    btn.classList.remove('copied');
                }, 2000);
            };

            if (navigator.clipboard && window.isSecureContext) {
                navigator.clipboard.writeText(currentIp).then(onCopied);
            } else {
                // Fallback pentru captive portal-uri HTTP
                const tempInput = document.createElement('input');
                tempInput.value = currentIp;
                document.body.appendChild(tempInput);
                tempInput.select();
                try {
                    document.execCommand('copy');
                    onCopied();
                } catch (err) {
                    console.error('Copy failed', err);
                }
                document.body.removeChild(tempInput);
            }
        }
    </script>
</body>
</html>
)rawliteral";

// --------------------------------------------------------------------------
// OTA FIRMWARE UPLOAD PORTAL
// --------------------------------------------------------------------------

const char *ota_portal = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="theme-color" content="#12161c">
    <meta name="color-scheme" content="dark">
    <link rel="icon" href="data:,">
    <title>EA-236 Firmware Update</title>
    <style>
        :root {
            --bg: #12161c;
            --panel: #1b222b;
            --panel-edge: #2b3441;
            --accent: #35b8ff;
            --good: #4fae7c;
            --bad: #d6543c;
            --text: #ece7dc;
            --muted: #8791a0;
            --mono: ui-monospace, SFMono-Regular, Consolas, "Roboto Mono", monospace;
            --sans: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
        }
        * { box-sizing: border-box; }
        body {
            margin: 0;
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 24px;
            background:
                radial-gradient(circle at 50% 0%, #1a212b 0%, var(--bg) 60%),
                repeating-linear-gradient(0deg, rgba(255, 90, 70, .025) 0px, rgba(255, 90, 70, .025) 1px, transparent 1px, transparent 24px),
                repeating-linear-gradient(90deg, rgba(255, 90, 70, .025) 0px, rgba(255, 90, 70, .025) 1px, transparent 1px, transparent 24px);
            font-family: var(--sans);
            color: var(--text);
            -webkit-tap-highlight-color: transparent;
        }
        .plate {
            width: min(400px, 92vw);
            background: var(--panel);
            border: 1px solid var(--panel-edge);
            border-radius: 10px;
            padding: 28px 26px 22px;
            box-shadow: 0 24px 60px rgba(0, 0, 0, .45);
            animation: rise .5s ease both;
        }
        @keyframes rise {
            from { opacity: 0; transform: translateY(12px); }
            to { opacity: 1; transform: translateY(0); }
        }
        .nameplate { display: flex; flex-direction: column; gap: 4px; margin-bottom: 18px; }
        .nameplate-id {
            display: flex;
            align-items: center;
            gap: 9px;
            font-family: var(--mono);
            font-size: 20px;
            letter-spacing: .04em;
            font-weight: 600;
        }
        .nameplate-sub {
            font-family: var(--mono);
            font-size: 11px;
            letter-spacing: .12em;
            text-transform: uppercase;
            color: var(--muted);
            padding-left: 19px;
        }
        .lamp {
            width: 10px;
            height: 10px;
            border-radius: 50%;
            background: var(--accent);
            box-shadow: 0 0 8px 1px var(--accent);
            animation: pulse 2.4s ease-in-out infinite;
            flex-shrink: 0;
        }
        .lamp--connecting { animation: pulse 1s ease-in-out infinite; }
        .lamp--ok { background: var(--good); box-shadow: 0 0 10px 2px var(--good); animation: none; }
        .lamp--err { background: var(--bad); box-shadow: 0 0 8px 2px var(--bad); animation: pulse 1.4s ease-in-out infinite; }
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: .4; }
        }
        .hairline {
            height: 1px;
            margin: 18px 0;
            background: linear-gradient(90deg, transparent, var(--panel-edge) 20%, var(--panel-edge) 80%, transparent);
        }
        h1 { font-family: var(--mono); font-size: 17px; letter-spacing: .02em; margin: 0 0 8px; }
        .lede { font-size: 13.5px; line-height: 1.5; color: var(--muted); margin: 0 0 20px; }
        .lede.warn { color: var(--accent); }
        .file-drop {
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 22px 14px;
            margin-bottom: 14px;
            border: 1px dashed var(--panel-edge);
            border-radius: 6px;
            font-size: 13.5px;
            color: var(--muted);
            text-align: center;
            cursor: pointer;
            transition: border-color .15s, color .15s;
        }
        .file-drop:hover { border-color: var(--accent); color: var(--text); }
        button[type="submit"] {
            width: 100%;
            padding: 13px;
            border: none;
            border-radius: 6px;
            background: var(--accent);
            color: #062736;
            font-family: var(--mono);
            font-size: 13px;
            font-weight: 700;
            letter-spacing: .08em;
            text-transform: uppercase;
            cursor: pointer;
            transition: filter .15s, transform .08s;
        }
        button[type="submit"]:hover:not(:disabled) { filter: brightness(1.08); }
        button[type="submit"]:active:not(:disabled) { transform: translateY(1px); }
        button[type="submit"]:disabled { opacity: .5; cursor: not-allowed; }
        .progress-track {
            height: 6px;
            margin-top: 16px;
            border-radius: 3px;
            background: var(--bg);
            border: 1px solid var(--panel-edge);
            overflow: hidden;
        }
        .progress-fill { height: 100%; width: 0%; background: var(--accent); transition: width .2s ease; }
        .status-line {
            display: flex;
            align-items: center;
            gap: 8px;
            min-height: 18px;
            margin: 12px 0 0;
            font-size: 13px;
            color: var(--muted);
        }
        .status-dot { width: 7px; height: 7px; border-radius: 50%; background: var(--panel-edge); flex-shrink: 0; }
        .status-dot--connecting { background: var(--accent); animation: pulse 1s ease-in-out infinite; }
        .status-dot--ok { background: var(--good); animation: none; }
        .status-dot--err { background: var(--bad); animation: none; }
        .plate-footer { display: flex; flex-direction: column; align-items: center; gap: 14px; }
        .btn-outline {
            display: block;
            width: 100%;
            text-align: center;
            padding: 11px;
            border: 1px solid var(--panel-edge);
            border-radius: 6px;
            color: var(--muted);
            font-family: var(--mono);
            font-size: 12px;
            letter-spacing: .06em;
            text-transform: uppercase;
            text-decoration: none;
            cursor: pointer;
            background: transparent;
            transition: border-color .15s, color .15s;
        }
        .btn-outline:hover { border-color: var(--accent); color: var(--text); }
        .credit { margin: 0; font-family: var(--mono); font-size: 10.5px; color: var(--muted); opacity: .6; }
        @media (prefers-reduced-motion: reduce) {
            .plate, .lamp, .status-dot { animation: none !important; }
        }
    </style>
</head>
<body>
    <main class="plate">
        <header class="nameplate">
            <div class="nameplate-id">
                <span class="lamp" id="lamp"></span>
                <span>EA-236</span>
            </div>
            <span class="nameplate-sub">Firmware Update</span>
        </header>

        <div class="hairline"></div>

        <section>
            <h1>Upload firmware</h1>
            <p class="lede warn">Keep EA-236 powered on during the update, interrupting it can leave the unit unable to boot.</p>

            <form id="otaForm">
                <label class="file-drop" id="fileDrop" for="fileInput">
                    <span id="fileLabel">Choose a .bin file</span>
                </label>
                <input type="file" id="fileInput" name="upload" accept=".bin" required hidden>
                <button type="submit" id="uploadBtn" disabled>Upload firmware</button>
            </form>

            <div class="progress-track">
                <div class="progress-fill" id="progressFill"></div>
            </div>

            <p class="status-line" id="statusLine">
                <span class="status-dot" id="statusDot"></span>
                <span id="statusText">No file selected</span>
            </p>
        </section>

        <div class="hairline"></div>

        <footer class="plate-footer">
            <a class="btn-outline" href="/">Back to setup</a>
            <p class="credit">Designed by teoelectric</p>
        </footer>
    </main>

    <script>
        (function () {
            const form = document.getElementById('otaForm');
            const fileInput = document.getElementById('fileInput');
            const fileLabel = document.getElementById('fileLabel');
            const uploadBtn = document.getElementById('uploadBtn');
            const progressFill = document.getElementById('progressFill');
            const statusText = document.getElementById('statusText');
            const statusDot = document.getElementById('statusDot');
            const lamp = document.getElementById('lamp');

            function setStatus(state, message) {
                statusDot.className = 'status-dot status-dot--' + state;
                lamp.className = 'lamp lamp--' + state;
                statusText.textContent = message;
            }

            fileInput.addEventListener('change', function () {
                if (fileInput.files.length > 0) {
                    fileLabel.textContent = fileInput.files[0].name;
                    uploadBtn.disabled = false;
                    setStatus('idle', 'Ready to upload');
                }
            });

            form.addEventListener('submit', function (event) {
                event.preventDefault();
                if (fileInput.files.length === 0) return;

                uploadBtn.disabled = true;
                fileInput.disabled = true;
                setStatus('connecting', 'Uploading, do not power off EA-236');

                const file = fileInput.files[0];
                const request = new XMLHttpRequest();

                request.upload.addEventListener('progress', function (event) {
                    if (event.lengthComputable) {
                        const percent = Math.round((event.loaded / event.total) * 100);
                        progressFill.style.width = percent + '%';
                    }
                });

                request.addEventListener('load', function () {
                    if (request.status === 200) {
                        progressFill.style.width = '100%';
                        setStatus('ok', 'Upload complete, EA-236 is restarting');
                    } else {
                        uploadBtn.disabled = false;
                        fileInput.disabled = false;
                        setStatus('err', 'Upload failed, try again');
                    }
                });

                request.addEventListener('error', function () {
                    uploadBtn.disabled = false;
                    fileInput.disabled = false;
                    setStatus('err', 'Connection lost during upload');
                });

                request.open('POST', '/upload');
                request.setRequestHeader('Content-Type', 'application/octet-stream');
                request.send(file);
            });
        })();
    </script>
</body>
</html>
)rawliteral";