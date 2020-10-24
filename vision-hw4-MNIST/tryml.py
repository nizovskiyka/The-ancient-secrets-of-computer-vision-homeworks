from uwimg import *

def softmax_model(inputs, outputs):
    l = [make_layer(inputs, outputs, SOFTMAX)]
    return make_model(l)

def neural_net(inputs, outputs):
    print(inputs)
    l = [   make_layer(inputs, 32, LOGISTIC),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

def neural_net2(inputs, outputs):
    print(inputs)
    l = [   make_layer(inputs, 64, RELU),
            make_layer(64, 32, RELU),
            make_layer(32, outputs, SOFTMAX)]
    return make_model(l)

print("loading data...")
train = load_classification_data("mnist.train", "mnist.labels", 1)
test  = load_classification_data("mnist.test", "mnist.labels", 1)
print("done")
print

print("training model...")
batch = 128
iters = 3000
#rate = .01
rate = 1.e-7
momentum = .9
decay = 0
#decay = 1.e4


#m = softmax_model(train.X.cols, train.y.cols)
m = neural_net2(train.X.cols, train.y.cols)
#m = neural_net2(train.X.cols, train.y.cols)
train_model(m, train, batch, iters, rate, momentum, decay)
print("done")
print

print("evaluating model...")

print("training accuracy: %f", accuracy_model(m, train))
print("test accuracy:     %f", accuracy_model(m, test))

with open("research/decay-nn2.log", "a") as file:
    file.write('decay: ' + str(decay) + ' tr acc: ' + str(accuracy_model(m, train)) + ' test acc: ' + str(accuracy_model(m, test)))
    file.write("\n")


## Questions ##

# 2.2.1 Why might we be interested in both training accuracy and testing accuracy? What do these two numbers tell us about our current model?
# Training accuracy contains information about how effective is the algorithm, testing accuracy - how well the model is learned;

# 2.2.2 Try varying the model parameter for learning rate to different powers of 10 (i.e. 10^1, 10^0, 10^-1, 10^-2, 10^-3) and training the model. What patterns do you see and how does the choice of learning rate affect both the loss during training and the final model accuracy?
# Rate: > 0.01 -> model runs to -nan, < 0.01 -> accuracy decreases

# 2.2.3 Try varying the parameter for weight decay to different powers of 10: (10^0, 10^-1, 10^-2, 10^-3, 10^-4, 10^-5). How does weight decay affect the final model training and test accuracy?
# At rate = 0.01 accuracies increase with decay reaching 1.e-4

# 2.3.1 Currently the model uses a logistic activation for the first layer. Try using a the different activation functions we programmed. How well do they perform? What's best?
# They all are bad (loss -> -nan) except logistic (which seems not to learn at all)

# 2.3.2 Using the same activation, find the best (power of 10) learning rate for your model. What is the training accuracy and testing accuracy?
# rate: 1e-07 training accuracy: 0.87985 test accuracy: 0.879

# 2.3.3 Right now the regularization parameter `decay` is set to 0. Try adding some decay to your model. What happens, does it help? Why or why not may this be?
# yes, apparently decay = 1000000 is the best value

# 2.3.4 Modify your model so it has 3 layers instead of two. The layers should be `inputs -> 64`, `64 -> 32`, and `32 -> outputs`. Also modify your model to train for 3000 iterations instead of 1000. Look at the training and testing error for different values of decay (powers of 10, 10^-4 -> 10^0). Which is best? Why?
# On the model with 2 RELU layers the decay does no affect on the model (increasing the decay decreases the accuracy)

# 3.2.1 How well does your network perform on the CIFAR dataset?
#Training accuracy: 0.3746, test accuracy: 0.3748 at rate = 1e-3