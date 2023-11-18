#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// 神经网络的结构体
typedef struct {
    double *weights; // 权重
    double bias; // 偏置
    double (*activation)(double); // 激活函数
} NeuralNetwork;

// 激活函数：sigmoid
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
double dot_product(double *weights,double *inputs){
    int i=0;
    double ans=0;
    while(i<2){
        ans+=weights[i]*inputs[i];
        i++;
    }
    return ans;
}
double sigmoid_derivative(double output){
    double ans=output*(1-output);//sigmoid函数的导数
    return ans;
}
// 训练神经网络
void train(NeuralNetwork *nn, double *inputs, double *expected_outputs) {
    double output = nn->activation(dot_product(nn->weights, inputs) + nn->bias); // 计算输出
    double error = expected_outputs[0] - output; // 计算误差
    double gradients[1] = {error * sigmoid_derivative(output)}; // 计算梯度
    for(int i=0; i<nn->weights[0]; i++) { // 更新权重和偏置
        nn->weights[i+1] += gradients[0] * inputs[i];
        nn->bias += gradients[0];
    }
}
int main() {
    NeuralNetwork nn = {.weights = malloc(3 * sizeof(double)), .bias = 0.1}; // 初始化神经网络，权重初始化为0，偏置初始化为0
    nn.weights[0]=1;
    nn.weights[1]=0;
    nn.weights[2]=0;
    nn.activation=sigmoid;
    double inputs[2] = {1, 1}; // 输入数据
    double expected_outputs[1] = {1}; // 期望的输出数据
    for(int i=0; i<10000; i++) { // 进行10000次训练
        train(&nn, inputs, expected_outputs);
    }
    double output = nn.activation(dot_product(nn.weights, inputs) + nn.bias); // 计算实际的输出
    printf("Output: %.7f\n", output); // 输出结果
    free(nn.weights); // 释放内存
    return 0;
}