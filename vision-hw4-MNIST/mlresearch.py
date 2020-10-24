from uwimg import *

def softmax_model(inputs, outputs):
    l = [make_layer(inputs, outputs, SOFTMAX)]
    return make_model(l)

def neural_net(inputs, outputs, activation = LOGISTIC):
    print(inputs)
    l = [   make_layer(inputs, 32, activation),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

def neural_net2(inputs, outputs):
    print(inputs)
    l = [   make_layer(inputs, 64, LOGISTIC),
            make_layer(64, 32, LOGISTIC),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

print("loading data...")
train = load_classification_data("mnist.train", "mnist.labels", 1)
test  = load_classification_data("mnist.test", "mnist.labels", 1)
print("done")
print

#print("training model...")
batch = 128
iters = 1000
rate = .01
rates = [10.0, 1, 0.1, 0.01, 0.001]
momentum = .9
decay = .0
decays = [1, 0.1, 0.01, 0.001, 0.0001, 0.00001]
activations = [LOGISTIC, LINEAR, SOFTMAX, RELU, LRELU]

print("===SOFTMAX MODEL===")
m = softmax_model(train.X.cols, train.y.cols)
# m = neural_net2(train.X.cols, train.y.cols)
# train_model(m, train, batch, iters, rate, momentum, decay)
for r in rates:
    train_model(m, train, batch, iters, r, momentum, decay)
    with open('research/research-rates.log', 'a') as file:
        file.write('rate: ' + str(r) + ' tr acc: ' + str(accuracy_model(m, train)) + ' test acc: ' + str(accuracy_model(m, test)))
        file.write("\n")
print('rates done')

for d in decays:
    train_model(m, train, batch, iters, rate, momentum, d)
    with open('research/research-decays.log', 'a') as file:
        file.write('decay: ' + str(d) + ' tr acc: ' + str(accuracy_model(m, train)) + ' test acc: ' + str(accuracy_model(m, test)))
        file.write("\n")
print('decays done')

print("===Neural Network 1===")
for act in activations:
    m = neural_net(train.X.cols, train.y.cols, activation=act)
    train_model(m, train, batch, iters, rate, momentum, decay)
    with open('research/research-activations.log', 'a') as file:
        file.write('activation: ' + str(act) + ' tr acc: ' + str(accuracy_model(m, train)) + ' test acc: ' + str(accuracy_model(m, test)))
        file.write("\n")
print('activations done')

# print("evaluating model...")

# print("training accuracy: %f", accuracy_model(m, train))
# print("test accuracy:     %f", accuracy_model(m, test))

# with open("nn_decay_research.log", "a") as file:
#     file.write("rate: " + str(decay) +'\n')
#     file.write("training accuracy: " + str(accuracy_model(m, train))+'\n')
#     file.write("test accuracy:     " + str(accuracy_model(m, test))+'\n')
#     file.write("**********************************************************************"+'\n')


## Questions ##

# 2.2.1 Why might we be interested in both training accuracy and testing accuracy? What do these two numbers tell us about our current model?
# Training accuracy contains information about how effective is the algorithm, testing accuracy - how well the model is learned;

# 2.2.2 Try varying the model parameter for learning rate to different powers of 10 (i.e. 10^1, 10^0, 10^-1, 10^-2, 10^-3) and training the model. What patterns do you see and how does the choice of learning rate affect both the loss during training and the final model accuracy?
# Rate: > 0.01 -> model runs to nan, < 0.01 -> accuracy decreases

# 2.2.3 Try varying the parameter for weight decay to different powers of 10: (10^0, 10^-1, 10^-2, 10^-3, 10^-4, 10^-5). How does weight decay affect the final model training and test accuracy?
# At rate = 0.01 accuracies increase with decay reaching 0

# 2.3.1 Currently the model uses a logistic activation for the first layer. Try using a the different activation functions we programmed. How well do they perform? What's best?

# 2.3.2 Using the same activation, find the best (power of 10) learning rate for your model. What is the training accuracy and testing accuracy?


# 2.3.3 Right now the regularization parameter `decay` is set to 0. Try adding some decay to your model. What happens, does it help? Why or why not may this be?


# 2.3.4 Modify your model so it has 3 layers instead of two. The layers should be `inputs -> 64`, `64 -> 32`, and `32 -> outputs`. Also modify your model to train for 3000 iterations instead of 1000. Look at the training and testing error for different values of decay (powers of 10, 10^-4 -> 10^0). Which is best? Why?


# 3.2.1 How well does your network perform on the CIFAR dataset?


