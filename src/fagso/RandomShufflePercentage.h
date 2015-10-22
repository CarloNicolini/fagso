#ifndef RANDOMSHUFFLEPERCENTAGE_H
#define RANDOMSHUFFLEPERCENTAGE_H
#include <vector>
#include <algorithm>

template<class T>
class RandomShufflePercentageIterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
    RandomShufflePercentageIterator(std::vector<T>& vec, size_t pos = 0): myVec(vec), myIndex(0), myPos(pos)
    {
        srand(time(NULL));
    }

    bool operator==(const RandomShufflePercentageIterator& rhs) const
    {
        return myPos == rhs.myPos;
    }

    bool operator!=(const RandomShufflePercentageIterator& rhs) const
    {
        return ! (myPos == rhs.myPos);
    }

    bool operator<(const RandomShufflePercentageIterator& rhs) const
    {
        return myPos < rhs.myPos;
    }

    RandomShufflePercentageIterator& operator++()
    {
        ++myPos;
        return fill();
    }

    RandomShufflePercentageIterator& operator++(int)
    {
        ++myPos;
        return fill();
    }

    RandomShufflePercentageIterator& operator--()
    {
        --myPos;
        return fill();
    }

    RandomShufflePercentageIterator& operator--(int)
    {
        --myPos;
        return fill();
    }

    RandomShufflePercentageIterator& operator+(size_t n)
    {
        ++myPos;
        return fill();
    }

    RandomShufflePercentageIterator& operator-(size_t n)
    {
        --myPos;
        return fill();
    }

    const T& operator*() const
    {
        return myVec[myIndex];
    }

    T& operator*()
    {
        return myVec[myIndex];
    }

private:
    RandomShufflePercentageIterator& fill()
    {
        myIndex = rand() % myVec.size();
        return *this;
    }

private:
    size_t myIndex;
    std::vector<T>& myVec;
    size_t myPos;
};
#endif // RANDOMSHUFFLEPERCENTAGE_H
