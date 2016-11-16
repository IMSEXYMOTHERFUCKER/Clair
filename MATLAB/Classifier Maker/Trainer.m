 %load('positives.mat');   
 trainCascadeObjectDetector('HandBack.xml',data,'Negative','FeatureType','Haar','ObjectTrainingSize',[30,28],'NegativeSamplesFactor',3);   