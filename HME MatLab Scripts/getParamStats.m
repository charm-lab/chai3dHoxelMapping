% Returns the mean and std of values passed sorted by mapping
% getParamStats parameters must go in brackets due to being stored as cells

function [meanVals, stdVals, confInts] = getParamStats(map1, map3, map5)
% map1
% map3
% map5
numSubjects = evalin('base','numSubjects');
numTrialsPerMapping = evalin('base','numTrialsPerMapping');
p = evalin('base','p');
showInidividualSubjects = evalin('base','showInidividualSubjects');


if (showInidividualSubjects == false)
    % Calculate means and standard deviations for all subjects
    cols = [1:numSubjects];
else
    % Calculate means and standard deviations for each subject individually
    cols = 1;
end

% z for 95% confidence interval
z = 1.96; 
n = numSubjects*10;

if (p == 1)
    mapping1_mean = [mean(reshape(map1(1:10,cols),[],1)); mean(reshape(map1(11:20,cols),[],1))];
    mapping1_std = [std(reshape(map1(1:10,cols),[],1)); std(reshape(map1(11:20,cols),[],1))];
    mapping1_ci = z*[std(reshape(map1(1:10,cols),[],1)); std(reshape(map1(11:20,cols),[],1))]./sqrt(n);

    mapping3_mean = [mean(reshape(map3(1:10,cols),[],1)); mean(reshape(map3(11:20,cols),[],1))];
    mapping3_std = [std(reshape(map3(1:10,cols),[],1)); std(reshape(map3(11:20,cols),[],1))];
    mapping3_ci = z*[std(reshape(map3(1:10,cols),[],1)); std(reshape(map3(11:20,cols),[],1))]./sqrt(n);

    mapping5_mean = [mean(reshape(map5(1:10,cols),[],1)); mean(reshape(map5(11:20,cols),[],1))];
    mapping5_std = [std(reshape(map5(1:10,cols),[],1)); std(reshape(map5(11:20,cols),[],1))];
    mapping5_ci = z*[std(reshape(map5(1:10,cols),[],1)); std(reshape(map5(11:20,cols),[],1))]./sqrt(n);

else
    mapping1_mean = mean(reshape(map1(:,cols),[],1)); 
    mapping1_std = std(reshape(map1(:,cols),[],1)); 
    mapping1_ci = z*std(reshape(map1(:,cols),[],1))./sqrt(n); 

    mapping3_mean = mean(reshape(map3(:,cols),[],1));  
    mapping3_std = std(reshape(map3(:,cols),[],1));  
    mapping3_ci = z*std(reshape(map3(:,cols),[],1))./sqrt(n);  

    mapping5_mean = mean(reshape(map5(:,cols),[],1));  
    mapping5_std = std(reshape(map5(:,cols),[],1));  
    mapping5_ci = z*std(reshape(map5(:,cols),[],1))./sqrt(n);  

end
meanVals = [mapping1_mean', mapping3_mean',mapping5_mean'];
stdVals = [mapping1_std', mapping3_std', mapping5_std'];
confInts = [mapping1_ci', mapping3_ci', mapping5_ci'];

% meanVals = [mapping1_mean, mapping2_mean, mapping3_mean, mapping4_mean, mapping5_mean];
% stdVals = [mapping1_std, mapping2_std, mapping3_std, mapping4_std, mapping5_std];

end