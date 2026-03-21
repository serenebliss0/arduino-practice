// DOM Elements
const mainView = document.getElementById('dashboard-view');
const settingsView = document.getElementById('settings-view');
const volumeOverlay = document.getElementById('volume-overlay');
const connStatusText = document.getElementById('connection-status');

// Quick Settings Icons
const qsSettings = document.getElementById('qs-settings');
const qsHome = document.getElementById('qs-home');
const qsBt = document.getElementById('qs-bt');
const qsMic = document.getElementById('qs-mic');

// Mode & Mic Elements
const modeBtns = document.querySelectorAll('#dashboard-view .mode-btn');
const micContainer = document.getElementById('mic-status-container');
const micDot = document.getElementById('mic-dot');
const micStatusText = document.getElementById('mic-status-text');
const micSubtext = document.getElementById('mic-subtext');

// 1. Navigation (SPA Toggle)
qsSettings.addEventListener('click', () => {
    mainView.classList.add('hidden');
    settingsView.classList.remove('hidden');
});

qsHome.addEventListener('click', () => {
    settingsView.classList.add('hidden');
    mainView.classList.remove('hidden');
});

// Settings Tabs Logic
document.querySelectorAll('.settings-tab').forEach(tab => {
    tab.addEventListener('click', (e) => {
        // Remove active class from all tabs & hide all panes
        document.querySelectorAll('.settings-tab').forEach(t => t.classList.remove('active'));
        document.querySelectorAll('.settings-pane').forEach(p => p.classList.add('hidden'));
        
        // Activate clicked tab
        const targetBtn = e.currentTarget;
        targetBtn.classList.add('active');
        
        // Show target pane
        const targetPane = document.getElementById(targetBtn.dataset.target);
        if(targetPane) targetPane.classList.remove('hidden');
    });
});

// 2. Mode Switching & Subtext Updates
const modeTextMap = {
    'mode-bt': 'Connected via Bluetooth',
    'mode-smart': 'Using Smart Assistant',
    'mode-wifi': 'Connected via WiFi',
    'mode-aux': 'Connected via Headphone Jack'
};

modeBtns.forEach(btn => {
    btn.addEventListener('click', () => {
        modeBtns.forEach(b => b.classList.remove('active'));
        btn.classList.add('active');
        
        // Update Subtext immediately
        connStatusText.innerText = modeTextMap[btn.id];
        
        // Handle specific UI overlays based on mode
        if(btn.id === 'mode-aux') {
            volumeOverlay.classList.remove('hidden');
        } else {
            volumeOverlay.classList.add('hidden');
        }

        if(btn.id === 'mode-smart') {
            micContainer.classList.remove('hidden');
        } else {
            micContainer.classList.add('hidden');
        }
        
        // ESP32 API Hook: fetch(`/api/mode?set=${btn.id}`)
    });
});

// 3. Icon Toggles (Bluetooth & Mic)
qsBt.addEventListener('click', () => {
    qsBt.classList.toggle('active');
    // ESP32 API Hook: fetch(`/api/bt_toggle`)
});

qsMic.addEventListener('click', () => {
    qsMic.classList.toggle('danger');
    const isMuted = qsMic.classList.contains('danger');
    
    // Update Icon
    qsMic.innerHTML = isMuted ? '<i class="fas fa-microphone-slash"></i>' : '<i class="fas fa-microphone"></i>';
    
    // Update Smart Assistant Text if visible
    if(isMuted) {
        micDot.classList.replace('green', 'red');
        micStatusText.innerText = 'Microphone is off';
        micSubtext.classList.remove('hidden');
    } else {
        micDot.classList.replace('red', 'green');
        micStatusText.innerText = 'Microphone is on';
        micSubtext.classList.add('hidden');
    }
    // ESP32 API Hook: fetch(`/api/mic_toggle?muted=${isMuted}`)
});

// 4. Themes
function initTheme() {
    const savedTheme = localStorage.getItem('wavelet-theme') || 'default';
    document.documentElement.setAttribute('data-theme', savedTheme);
}

document.querySelectorAll('.theme-bubble').forEach(bubble => {
    bubble.addEventListener('click', (e) => {
        const selectedTheme = e.target.dataset.theme;
        document.documentElement.setAttribute('data-theme', selectedTheme);
        localStorage.setItem('wavelet-theme', selectedTheme);
    });
});

initTheme();