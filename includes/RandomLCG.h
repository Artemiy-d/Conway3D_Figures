#ifndef RANDOMLCG_H
#define RANDOMLCG_H

#include <time.h>

template <typename RandomGenerator>
class Probability
{
public:
    Probability( double _p = 0. )
        : m_value( _p * RandomGenerator::s_endValue  )
    { }

    double get() const
    {
        return 1. * m_value / RandomGenerator::s_endValue;
    }

    Probability & operator = ( double _p )
    {
        m_value = _p * RandomGenerator::s_endValue;
        return *this;
    }

    bool isLimit() const
    {
        return m_value == 0 || m_value == RandomGenerator::s_endValue;
    }

    bool simulate(RandomGenerator & _random) const
    {
        switch (m_value)
        {
        case 0:
            return false;
        case RandomGenerator::s_endValue:
            return true;
        default:

            return _random.next() < m_value;
        }
    }

    bool simulate( ) const
    {
        return simulate( s_random );
    }
private:
     unsigned int m_value;

     static RandomGenerator s_random;
};

template <unsigned int a, unsigned int b>
class RandomLCG
{
public:
    typedef ::Probability<RandomLCG> Probability;
public:
    RandomLCG() : m_value( time(0) )
    { }
    unsigned int next()
    {
        return (m_value = m_value * a + b) >> 1;
    }

    static const unsigned int s_maxValue = (~0u) >> 1;
    static const unsigned int s_endValue = s_maxValue + 1;
private:
    unsigned int m_value;
};

typedef RandomLCG<103515245, 12345> RandomLCGDefault;

#endif // RANDOMLCG_H
