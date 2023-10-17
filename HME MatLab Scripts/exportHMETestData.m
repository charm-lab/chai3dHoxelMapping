function [] = exportHMETestData(map1, map3, map5, metricName)

numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');
numSubjects =  evalin('base', 'numSubjects');
subjectNum = evalin('base','subjectNum');
numTrialsPerMapping =  evalin('base', 'numTrialsPerMapping');

for p = 1:numExperimentTypes
    if (p == 2)
        % Groups:
        % Mapping groups
        mappingsExp2(1:...
            numSubjects*numTrialsPerMapping(p), 1) = {'Mapping1'};
        mappingsExp2(numSubjects*numTrialsPerMapping(p) + 1:...
            2*numSubjects*numTrialsPerMapping(p), 1) = {'Mapping3'};
        mappingsExp2(2*numSubjects*numTrialsPerMapping(p)+1:...
            3*numSubjects*numTrialsPerMapping(p),1) = {'Control'};

    end
end

% Create matirx of strings naming the subjects for only one of the
% mappings, to be duplicated later:
for j = 1:numSubjects
    for i = 1:numTrialsPerMapping(p)
        subjects1Mapping{i,j} = strcat('Subject', num2str(subjectNum(j)));
    end
end

mappings = mappingsExp2;

subjects1Mapping = reshape(subjects1Mapping,[],1);

subjectsAllMappings = repmat(subjects1Mapping,3,1);

%Convert matrix of subject results to column vectors
p=2;
%CompletionTime Mapping 1 - Testingans
map1Test = reshape([map1{:,p}],[],1);
%CompletionTime Mapping 3 - Testing
map3Test  = reshape([map3{:,p}],[],1);
%CompletionTime Mapping 5 - Testing
map5Test  = reshape([map5{:,p}],[],1);

a = num2cell(map1Test);
b = num2cell(map3Test);
c = num2cell(map5Test);

data = [a;b;c];

combinedCell = [subjectsAllMappings mappings data];

myTable = array2table(combinedCell);
% myTable.Properties.VariableNames = ["SubjectNum","Mapping1","Mapping3","Control"];
myTable.Properties.VariableNames = ["SubjectNum","Mappings", "SubjectData"];

fileName = strcat('..\HME_R_Analysis\HMEData-for-R\', metricName, '.xlsx');

writetable(myTable,fileName);

disp(strcat(metricName," Test Data File Write Complete!"))

end