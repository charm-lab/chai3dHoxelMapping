% Returns the mean and std of values passed sorted by mapping
% getParamStats parameters must go in brackets due to being stored as cells

function [meanVals, stdVals] = getParamStats(map1, map3, map5)
% map1
% map3
% map5
numSubjects = evalin('base','numSubjects');
p = evalin('base','p');
%Calculate means and standard deviations for each subject
cols = [1:numSubjects]; %visCubeSubjectsRange

if (p == 1)
    mapping1_mean = [mean(reshape(map1(1:10,cols),[],1)); mean(reshape(map1(11:20,cols),[],1))];
    mapping1_std = [std(reshape(map1(1:10,cols),[],1)); std(reshape(map1(11:20,cols),[],1))];
    mapping3_mean = [mean(reshape(map3(1:10,cols),[],1)); mean(reshape(map3(11:20,cols),[],1))];
    mapping3_std = [std(reshape(map3(1:10,cols),[],1)); std(reshape(map3(11:20,cols),[],1))];
    mapping5_mean = [mean(reshape(map5(1:10,cols),[],1)); mean(reshape(map5(11:20,cols),[],1))];
    mapping5_std = [std(reshape(map5(1:10,cols),[],1)); std(reshape(map5(11:20,cols),[],1))];
else
    mapping1_mean = mean(reshape(map1(:,cols),[],1)); 
    mapping1_std = std(reshape(map1(:,cols),[],1)); 
    mapping3_mean = mean(reshape(map3(:,cols),[],1));  
    mapping3_std = std(reshape(map3(:,cols),[],1));  
    mapping5_mean = mean(reshape(map5(:,cols),[],1));  
    mapping5_std = std(reshape(map5(:,cols),[],1));  
end
meanVals = [mapping1_mean', mapping3_mean',mapping5_mean'];
stdVals = [mapping1_std', mapping3_std', mapping5_std'];

% meanVals = [mapping1_mean, mapping2_mean, mapping3_mean, mapping4_mean, mapping5_mean];
% stdVals = [mapping1_std, mapping2_std, mapping3_std, mapping4_std, mapping5_std];

end