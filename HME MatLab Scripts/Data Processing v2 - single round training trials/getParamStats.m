% Returns the mean and std of values passed sorted by mapping
% getParamStats parameters must go in brackets due to being stored as cells

function [meanVals, stdVals] = getParamStats(map1, map3, map5)
% map1
% map3
% map5
numSubjects = evalin('base','numSubjects');

%Calculate means and standard deviations for each subject
cols = [1:numSubjects]; %visCubeSubjectsRange 
mapping1_mean = mean(map1(:,cols)); mapping1_std = std(map1(:,cols));
mapping3_mean = mean(map3(:,cols)); mapping3_std = std(map3(:,cols));
mapping5_mean = mean(map5(:,cols)); mapping5_std = std(map5(:,cols));

% meanVals = [mapping1_mean, mapping2_mean, mapping3_mean, mapping4_mean, mapping5_mean];
% stdVals = [mapping1_std, mapping2_std, mapping3_std, mapping4_std, mapping5_std];

meanVals = [mapping1_mean', mapping3_mean',mapping5_mean'];
stdVals = [mapping1_std', mapping3_std', mapping5_std'];

end