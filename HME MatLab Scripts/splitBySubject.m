function [smallerVectors] = splitBySubject(originalVector)

numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');
numSubjects =  evalin('base', 'numSubjects');
subjectNum = evalin('base','subjectNum');
numTrialsPerMapping =  evalin('base', 'numTrialsPerMapping');

% Initialize variables to store the smaller vectors
numSubVectors = numSubjects;  % Number of smaller vectors
subVectorSize = numTrialsPerMapping(2);  % Size of each smaller vector
smallerVectors = cell(numSubVectors, 1);

% Split the original vector into smaller vectors
for i = 1:numSubVectors
    startIndex = (i - 1) * subVectorSize + 1;
    endIndex = i * subVectorSize;
    smallerVectors{i} = originalVector(startIndex:endIndex);
end

% Display the smaller vectors
for i = 1:numSubVectors
    disp(strcat('Subject ', num2str(i) ,' Data:'));
    disp(smallerVectors{i});
end

end