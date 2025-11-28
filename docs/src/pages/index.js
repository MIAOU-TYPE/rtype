import React from 'react';
import Layout from '@theme/Layout';
import clsx from 'clsx';
import styles from './index.module.css';

export default function Home() {
    return (
        <Layout
            title="R-TYPE Documentation"
            description="Technical documentation for the R-TYPE game engine and networked multiplayer project."
        >
            <header className={clsx(styles.heroBanner)}>
                <div className={styles.heroContainer}>
                    <h1 className={styles.title}>R-TYPE</h1>
                    <p className={styles.subtitle}>
                        A Multiplayer Shoot’em Up — explore the engine, network protocol, and gameplay systems.
                    </p>
                </div>
            </header>

            <main className={styles.main}>
                {/* About Section */}
                <section className={styles.section}>
                    <h2>⮕ What is R-TYPE?</h2>
                    <p>
                        <strong>R-TYPE</strong> is a classic horizontal shoot’em up reimagined as a multiplayer game.
                        Players face waves of enemies in real time, while a dedicated <strong>C++ server</strong> manages
                        all game logic, projectiles, and networking.
                    </p>
                    <ul>
                        <li><strong>Multiplayer:</strong> Real-time gameplay over UDP with server authority.</li>
                        <li><strong>Custom Engine:</strong> Built from scratch with ECS, Renderer, Audio, Physics, and AI.</li>
                        <li><strong>Cross-Platform:</strong> Runs on Linux & Windows, scalable architecture for future features.</li>
                    </ul>
                </section>

                <section className={styles.section}>
                    <h2>📘 Documentation Structure</h2>
                    <p>
                        The documentation is divided into main topics for developers and contributors:
                    </p>
                    <ul>
                        <li>
                            ECS, Rendering, Audio, Physics, AI.
                        </li>
                        <li>
                            UDP protocol, message reliability, server-client architecture.
                        </li>
                        <li>
                            Levels, enemies, bosses, weapons, and scoring.
                        </li>
                    </ul>
                </section>

                <section className={styles.section}>
                    <h2>⚙️ Getting Started</h2>
                    <ol>
                        <li>Clone the repository: <code>git clone &lt;repo-url&gt;</code></li>
                        <li>Install dependencies: <code>cmake . && make</code> for server and client.</li>
                        <li>Run the server: <code>./r-type_server</code></li>
                        <li>Run the client: <code>./r-type_client</code></li>
                        <li>Connect multiple clients to start multiplayer sessions.</li>
                    </ol>
                </section>

                <section className={styles.section}>
                    <h2>🧩 Project Goals</h2>
                    <ul>
                        <li>Learn advanced C++ and software engineering practices.</li>
                        <li>Design a modular and reusable game engine.</li>
                        <li>Implement a robust multiplayer networking system.</li>
                        <li>Create an enjoyable and faithful R-TYPE experience.</li>
                    </ul>
                </section>
            </main>
        </Layout>
    );
}
