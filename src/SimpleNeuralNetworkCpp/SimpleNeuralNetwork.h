/*
MIT License

Copyright (c) 2022 Evgenii Sopov (mrseakg@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef __SIMPLE_NEURAL_NETWORK_H__
#define __SIMPLE_NEURAL_NETWORK_H__

#include <vector>

class SimpleNeuralNetwork {
    public:
        explicit SimpleNeuralNetwork(std::vector<int> vLayers);
        const std::vector<float> &calc(const std::vector<float> &m_vInput);
        long long getCalcAvarageTimeInNanoseconds();
        const std::vector<float> &getGenom();
        void setGenom(const std::vector<float> &vWeights);
        void mutateGenom();
        void mixGenom(const std::vector<float> &vWeights);

    private:
        float randomWeight();
        const std::vector<int> m_vLayers;
        std::vector<float> m_vWeights{};
        std::vector<float> m_vBufferOutput{};
        std::vector<float> m_vBufferSignals{};
        long long m_nCalcSumMs;
        int m_nCalcCounter;
        int m_nInputSize;
        int m_nLayersSize;
        int m_nOutputSize;
};

class SimpleNeuralTrainingItem {
    public:
        explicit SimpleNeuralTrainingItem(const std::vector<float> in, const std::vector<float> out);

        const std::vector<float> &getIn() const;
        const std::vector<float> &getOut() const;

    private:
        const std::vector<float> m_vIn;
        const std::vector<float> m_vOut;
};

class SimpleNeuralTrainingItemList {
    public:
        SimpleNeuralTrainingItemList(int nNumberOfIn, int nNumberOfOut);

        int getNumberOfIn() const;
        int getNumberOfOut() const;
        void addItem(std::vector<float> in, std::vector<float> out);
        unsigned int size() const;
        std::vector<SimpleNeuralTrainingItem>::iterator begin();
        std::vector<SimpleNeuralTrainingItem>::iterator end();

    private:
        int m_nNumberOfIn;
        int m_nNumberOfOut;
        std::vector<SimpleNeuralTrainingItem> m_vData;
};


class SimpleNeuralGenom {
    public:
        explicit SimpleNeuralGenom(std::vector<float> vGenom, float nRating);

        const std::vector<float> &getGenom() const;
        void setGenom(const std::vector<float> &vGenom);

        float getRating() const;
        void addRating(float nDiff);
        void setRating(float nRating);

        void calculateRating(SimpleNeuralNetwork *pNet, SimpleNeuralTrainingItemList *pTrainingData);

    private:
        float m_nRating;
        std::vector<float> m_vGenom;
};


class SimpleNeuralGenomList {
    public:
        SimpleNeuralGenomList(int nBetter, int nMutate, int nMix);
        void fillRandom(SimpleNeuralNetwork *pNet);

        const std::vector<SimpleNeuralGenom> &list() const;
        float getBetterRating();
        void sort();
        void printFirstRatings(int nNumber);
        void mutateAndMix(SimpleNeuralNetwork *pNet);

        const SimpleNeuralGenom &getBetterGenom();

        void calculateRatingForAll(SimpleNeuralNetwork *pNet, SimpleNeuralTrainingItemList *pTrainingData);
        void calculateRatingForMutatedAndMixed(SimpleNeuralNetwork *pNet, SimpleNeuralTrainingItemList *pTrainingData);

    private:
        int m_nBetterGenoms;
        int m_nMutateGenoms;
        int m_nMixGenoms;
        int m_nAllGenoms;
        
        std::vector<SimpleNeuralGenom> m_vGenoms;
};

#endif // __SIMPLE_NEURAL_NETWORK_H__