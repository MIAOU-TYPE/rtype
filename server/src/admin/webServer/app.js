function authHeaders() {
    const t = document.getElementById("token").value.trim();
    return { "Authorization": "Bearer " + t };
}

async function apiGet(path) {
    const r = await fetch(path, { headers: authHeaders() });
    if (!r.ok) throw new Error(await r.text());
    return r.json();
}

async function apiPost(path, body) {
    const r = await fetch(path, {
        method: "POST",
        headers: { ...authHeaders(), "Content-Type": "application/json" },
        body: JSON.stringify(body || {})
    });
    if (!r.ok) throw new Error(await r.text());
    return r.json();
}

function renderTable(el, cols, rows) {
    const t = document.getElementById(el);
    t.innerHTML = "";
    const thead = document.createElement("tr");
    cols.forEach(c => { const th = document.createElement("th"); th.textContent = c; thead.appendChild(th); });
    t.appendChild(thead);

    rows.forEach(r => {
        const tr = document.createElement("tr");
        cols.forEach(c => {
            const td = document.createElement("td");
            td.appendChild(r[c] instanceof Node ? r[c] : document.createTextNode(String(r[c] ?? "")));
            tr.appendChild(td);
        });
        t.appendChild(tr);
    });
}

async function refreshAll() {
    try {
        const status = await apiGet("/api/status");
        document.getElementById("status").textContent = JSON.stringify(status, null, 2);
        const rooms = await apiGet("/api/rooms");
        renderTable("rooms", ["roomId","name","players","maxPlayers"], rooms);
        const sessions = await apiGet("/api/sessions");
        const rows = sessions.map(s => {
            const kick = document.createElement("button");
            kick.textContent = "Kick";
            kick.onclick = async () => { await apiPost("/api/kick", { who: String(s.id) }); await refreshAll(); };

            const ban = document.createElement("button");
            ban.textContent = "Ban 60m";
            ban.onclick = async () => { await apiPost("/api/ban", { who: String(s.id), minutes: 60 }); await refreshAll(); };

            return {
                id: s.id,
                username: s.username,
                authed: s.authed,
                roomId: s.roomId,
                kick: kick,
                ban: ban
            };
        });
        renderTable("sessions", ["id","username","authed","roomId","kick","ban"], rows);

        const bans = await apiGet("/api/bans");
        renderTable("bans", ["ip","remainingSeconds"], bans);

    } catch (e) {
        alert("Error: " + e.message);
    }
}
