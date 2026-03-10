/* slider */

const slider = document.getElementById("slider");
const val = document.getElementById("val");

slider.addEventListener("input", () => {

    let v = parseFloat(slider.value);

    v = Math.round(v / 0.25) * 0.25;

    v = Math.min(2, Math.max(0, v));

    val.textContent = v.toFixed(2).replace(/\.00$/, "");

});

/* toggle */

const toggle = document.getElementById("toggle");
let on = false;

toggle.onclick = () => {
    on = !on;
    toggle.classList.toggle("on", on);
    toggle.textContent = on ? "ON" : "OFF";
};

/* Change button */

const changeBtn = document.getElementById("changeBtn");

changeBtn.onclick = () => {
//    alert("Change button clicked!"); // replace with your logic
};

/* background animation */

const canvas = document.getElementById("bg");
const ctx = canvas.getContext("2d");

let mouse = { x: -9999, y: -9999 };

const NUM_LINES = 20;
const POINTS = 140;
const MOUSE_RADIUS = 160;
const PUSH = 90;
const RETURN = 0.012;

let lines = [];

function build() {

    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

    lines = [];

    for (let i = 0; i < NUM_LINES; i++) {

        let y = canvas.height * ((i + 1) / (NUM_LINES + 1));

        let pts = [];

        let amplitude = 6 + Math.random() * 10;
        let frequency = 0.015 + Math.random() * 0.02;
        let phase = Math.random() * Math.PI * 2;

        for (let j = 0; j < POINTS; j++) {

            let x = (j / (POINTS - 1)) * canvas.width;

            let wave = Math.sin(x * frequency + phase) * amplitude;

            let noise = (Math.random() - 0.5) * 2;

            pts.push({
                bx: x,
                by: y + wave + noise,
                ox: 0,
                oy: 0
            });

        }

        lines.push(pts);

    }

}

window.addEventListener("resize", build);

window.addEventListener("mousemove", e => {
    mouse.x = e.clientX;
    mouse.y = e.clientY;
});

build();

function tick() {

    for (let line of lines) {

        for (let p of line) {

            let cx = p.bx + p.ox;
            let cy = p.by + p.oy;

            let dx = cx - mouse.x;
            let dy = cy - mouse.y;

            let dist = Math.sqrt(dx * dx + dy * dy);

            if (dist < MOUSE_RADIUS && dist > 0) {

                let f = Math.pow(1 - dist / MOUSE_RADIUS, 2) * PUSH;

                p.ox += (dx / dist) * f * 0.25;
                p.oy += (dy / dist) * f * 0.25;

            }

            p.ox *= 1 - RETURN;
            p.oy *= 1 - RETURN;

        }

    }

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    ctx.fillStyle = "#083338";
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    for (let i = 0; i < lines.length; i++) {

        let pts = lines[i];

        let alpha = 0.05 + (i % 4) * 0.018;

        ctx.beginPath();
        ctx.strokeStyle = `rgba(94,228,212,${alpha})`;
        ctx.lineWidth = 1.2;

        ctx.moveTo(pts[0].bx + pts[0].ox, pts[0].by + pts[0].oy);

        for (let j = 1; j < pts.length - 1; j++) {

            let c = pts[j];
            let n = pts[j + 1];

            let mx = (c.bx + c.ox + n.bx + n.ox) / 2;
            let my = (c.by + c.oy + n.by + n.oy) / 2;

            ctx.quadraticCurveTo(
                c.bx + c.ox,
                c.by + c.oy,
                mx,
                my
            );

        }

        let last = pts[pts.length - 1];

        ctx.lineTo(last.bx + last.ox, last.by + last.oy);

        ctx.stroke();

    }

    requestAnimationFrame(tick);

}

tick();