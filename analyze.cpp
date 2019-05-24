#include "analyze.h"
#include "weights.h"
#include <QFile>
#include <QVector>
#include <QDebug>

Analyze::Analyze(QObject *parent) : QObject(parent)
{
    mlp = new MultilayerPerceptron(4,1);
    mlp->addHiddenLayer(10);
    mlp->addHiddenLayer(10);
    mlp->init();
}

Analyze::~Analyze()
{
    delete mlp;
}

void Analyze::train(float error_, const QString& filename)
{
    QVector<QVector<float>> v;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "не открыт " << filename;
        return;
    }

    while (!file.atEnd()) {
        QVector<float> floats;
        QByteArray line = file.readLine();
        auto strings = line.split(',');
        for (auto s : strings)
        {
            floats.append(s.trimmed().toFloat());
        }

        v.append(floats);
    }

    v.remove(0);

    std::vector<MultilayerPerceptron::TrainingElement> trainingSet;

    for  (auto vv: v)
    {
        std::vector<float> teInput;
        for (int i = 0; i<vv.count()-1; i++)
        {
            teInput.push_back(normalize(vv[i], i));
        }
        std::vector<float> teOutput;
        teOutput.push_back(vv[vv.count()-1]);

        trainingSet.push_back(MultilayerPerceptron::TrainingElement(teInput, teOutput));
    }

    mlp->setTrainingSet(trainingSet);

    float error = 10;
    while (error > error_)
    {
        error = mlp->train(0.3f);
        qDebug() << error;
    }
}

void Analyze::loadWeights()
{
    QFile file("weights.bin");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug () << file.errorString();
        qDebug() << "weights не открыт";
        return;
    }

    std::vector<WeightMatrix> matrix;

    while (true)
    {
        Weights w;

        auto c = sizeof(Weights);
        auto readed = file.read((char*)&w, c);
        if (readed < c)
            break;
        WeightMatrix wm(w.ins,w.outs, 0);
        wm.inputDim = w.ins;
        wm.outputDim = w.outs;
        wm.w.clear();
        for (int i =0; i<wm.outputDim*wm.inputDim;i++)
            wm.w.push_back(w.w[i]);

        matrix.push_back(wm);
    }

    file.close();

    mlp->setWeights(matrix);
}

void Analyze::saveWeights()
{
    QFile file2("weights.bin");
    file2.open(QIODevice::WriteOnly);

    auto ew = mlp->getWeights();

    for (WeightMatrix wg : ew)
    {
        Weights w;
        memset(w.w,0x00,1000);
        w.ins = wg.inputDim;
        w.outs = wg.outputDim;

        for (int i = 0; i<wg.w.size(); i++)
            w.w[i] = wg.w[i];

        auto c = sizeof(w);
        file2.write((char*)&w,c);
        file2.flush();
    }

    file2.close();
}

float Analyze::calcResult(std::vector<float> &ins)
{
    std::vector<float> nIns;
    for (int i=0; i<ins.size(); i++)
        nIns.push_back(normalize(ins[i], i));

    auto result = *mlp->classify(nIns).begin();;
    return result;
}

float Analyze::normalize(float n, int i)
{
    float max;
    float min;

    switch (i)
    {
    case 0:
    {
        max = 100;
        min = 0;
        break;
    }
    case 1:
    {
        max = 1000000;
        min = 0;
        break;
    }
    case 2:
    {
        max = 1000;
        min = 0;
        break;
    }
    case 3:
    {
        max = 10;
        min = 0;
        break;
    }
    }

    float res = (n - min)*(0.9-0) / (max-min);

    return res;
}
