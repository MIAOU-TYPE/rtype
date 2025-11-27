import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const FeatureList = [
    {
        title: 'Multiplayer in Real Time',
        description: (
            <>
                R-TYPE is a real-time multiplayer shoot’em up based on UDP networking.
                A dedicated server manages players, enemies, and projectiles.
            </>
        ),
    },
    {
        title: 'Custom C++ Engine',
        description: (
            <>
                Built from scratch in <strong>C++20</strong> using a modular architecture:
                <br />
                Entity Component System, Renderer, Audio, Physics and AI.
            </>
        ),
    },
    {
        title: 'Cross-Platform & Scalable',
        description: (
            <>
                Designed to run on <strong>Linux & Windows</strong> (and macOS during
                development). Scalable architecture supporting future features.
            </>
        ),
    },
];

function Feature({ title, description }) {
    return (
        <div className={clsx('col col--4')}>
            <div className="text--center padding-horiz--md">
                <Heading as="h3">{title}</Heading>
                <p>{description}</p>
            </div>
        </div>
    );
}

export default function HomepageFeatures() {
    return (
        <section className={styles.features}>
            <div className="container">
                <div className="row">
                    {FeatureList.map((props, idx) => (
                        <Feature key={idx} {...props} />
                    ))}
                </div>
            </div>
        </section>
    );
}
x