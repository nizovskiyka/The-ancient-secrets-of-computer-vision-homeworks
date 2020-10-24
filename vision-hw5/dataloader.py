import torch
import torchvision
import torchvision.transforms as transforms


class CifarLoader(object):
	"""docstring for CifarLoader"""
	def __init__(self, args):
		super(CifarLoader, self).__init__()
		transform = transforms.Compose(
		    [
		    # TODO: Add data augmentations here
			transforms.ColorJitter(brightness=0.5, contrast=0.5, saturation=0.5, hue=0.2),
			transforms.RandomPerspective(distortion_scale=0.2),
			transforms.RandomAffine(degrees=40, scale=(.9, 1.1), shear=0),
			transforms.RandomHorizontalFlip(),
			transforms.RandomRotation(30),
		    transforms.ToTensor(),
		    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
			transforms.RandomErasing(scale=(0.02, 0.16), ratio=(0.3, 1.6)),
		    ])

		transform_test = transforms.Compose([
		    transforms.ToTensor(),
		    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
		])

		trainset = torchvision.datasets.CIFAR10(root='./data', train=True,
		                                        download=True, transform=transform)
		self.trainloader = torch.utils.data.DataLoader(trainset, batch_size=args.batchSize,
		                                          shuffle=True, num_workers=2)

		testset = torchvision.datasets.CIFAR10(root='./data', train=False,
		                                       download=True, transform=transform_test) 
		self.testloader = torch.utils.data.DataLoader(testset, batch_size=args.batchSize,
		                                         shuffle=False, num_workers=2)

		self.classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')
		
