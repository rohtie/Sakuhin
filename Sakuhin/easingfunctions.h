#ifndef EASINGFUNCTIONS_H
#define EASINGFUNCTIONS_H

#include <QtMath>

const double pi = qAcos(-1.0);

double Linear(double t) {
    return t;
}

double QuadraticIn(double t) {
    return t * t;
}

double QuadraticOut(double t) {
    return -(t * (t - 2));
}

double QuadraticInOut(double t) {
    if (t < 0.5) {
        return 2 * t * t;
    }
    else {
        return (-2 * t * t) + (4 * t) - 1;
    }
}

double CubicIn(double t) {
    return t * t * t;
}

double CubicOut(double t) {
    double f = (t - 1);
    return f * f * f + 1;
}

double CubicInOut(double t) {
    if (t < 0.5) {
        return 4 * t * t * t;
    }
    else {
        double f = ((2 * t) - 2);
        return 0.5 * f * f * f + 1;
    }
}

double QuarticIn(double t) {
    return t * t * t * t;
}

double QuarticOut(double t) {
    double f = (t - 1);
    return f * f * f * (1 - t) + 1;
}

double QuarticInOut(double t) {
    if (t < 0.5) {
        return 8 * t * t * t * t;
    }
    else {
        double f = (t - 1);
        return -8 * f * f * f * f + 1;
    }
}

double QuinticIn(double t) {
    return t * t * t * t * t;
}

double QuinticOut(double t) {
    double f = (t - 1);
    return f * f * f * f * f + 1;
}

double QuinticInOut(double t) {
    if (t < 0.5) {
        return 16 * t * t * t * t * t;
    }
    else {
        double f = ((2 * t) - 2);
        return  0.5 * f * f * f * f * f + 1;
    }
}

double SineIn(double t) {
    return qSin((t - 1) * pi) + 1;
}

double SineOut(double t) {
    return qSin(t * pi);
}

double SineInOut(double t) {
    return 0.5 * (1 - qCos(t * pi));
}

double CircularIn(double t) {
    return 1 - qSqrt(1 - (t * t));
}

double CircularOut(double t) {
    return qSqrt((2 - t) * t);
}

double CircularInOut(double t) {
    if (t < 0.5) {
        return 0.5 * (1 - qSqrt(1 - 4 * (t * t)));
    }
    else {
        return 0.5 * (qSqrt(-((2 * t) - 3) * ((2 * t) - 1)) + 1);
    }
}

double ExponentialIn(double t) {
    return (t == 0.0) ? t : qPow(2, 10 * (t - 1));
}

double ExponentialOut(double t) {
    return (t == 1.0) ? t : 1 - qPow(2, -10 * t);
}

double ExponentialInOut(double t) {
    if (t == 0.0 || t == 1.0) return t;

    if (t < 0.5) {
        return 0.5 * qPow(2, (20 * t) - 10);
    }
    else {
        return -0.5 * qPow(2, (-20 * t) + 10) + 1;
    }
}

double ElasticIn(double t) {
    return qSin(13 * pi * t) * qPow(2, 10 * (t - 1));
}

double ElasticOut(double t) {
    return qSin(-13 * pi * (t + 1)) * qPow(2, -10 * t) + 1;
}

double ElasticInOut(double t) {
    if (t < 0.5) {
        return 0.5 * qSin(13 * pi * (2 * t)) * qPow(2, 10 * ((2 * t) - 1));
    }
    else {
        return 0.5 * (qSin(-13 * pi * ((2 * t - 1) + 1)) * qPow(2, -10 * (2 * t - 1)) + 2);
    }
}

double BackIn(double t) {
    return t * t * t - t * qSin(t * pi);
}

double BackOut(double t) {
    double f = (1 - t);
    return 1 - (f * f * f - f * qSin(f * pi));
}

double BackInOut(double t) {
    if (t < 0.5) {
        double f = 2 * t;
        return 0.5 * (f * f * f - f * qSin(f * pi));
    }
    else {
        double f = (1 - (2*t - 1));
        return 0.5 * (1 - (f * f * f - f * qSin(f * pi))) + 0.5;
    }
}

double BounceOut(double t) {
    if (t < 4/11.0) {
        return (121 * t * t)/16.0;
    }
    else if (t < 8/11.0) {
        return (363/40.0 * t * t) - (99/10.0 * t) + 17/5.0;
    }
    else if (t < 9/10.0) {
        return (4356/361.0 * t * t) - (35442/1805.0 * t) + 16061/1805.0;
    }
    else {
        return (54/5.0 * t * t) - (513/25.0 * t) + 268/25.0;
    }
}

double BounceIn(double t) {
    return 1 - BounceOut(1 - t);
}

double BounceInOut(double t) {
    if (t < 0.5) {
        return 0.5 * BounceIn(t * 2);
    }
    else {
        return 0.5 * BounceOut(t * 2 - 1) + 0.5;
    }
}

#endif // EASINGFUNCTIONS_H
