% Hoxel Mapping Experiments (HME) Anova Analysis
% Author: Jasmin E. Palmer
function [p, tbl, stats] = runHMEANOVA1(map1, map3, map5, metricName)

numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');
numSubjects =  evalin('base', 'numSubjects');
subjectNum = evalin('base','subjectNum');
numTrialsPerMapping =  evalin('base', 'numTrialsPerMapping');

% Statistcal Tests v3 -- 1-way and 2-Way ANOVA
% p = anovan(y, group)
showStats = "on";
format long; % close all;
% fontSize = 10; width = 1700; height = 1000;
fontSize = 10; width = 700; height = 400;

% interactableCompare = true;
interactableCompare = false;

p=2;

if (metricName == "Total Cube Break Occurences")
    % Groups:
    % Mapping groups
    mappingsExp2(1:numSubjects, 1) = {'1'};
    mappingsExp2(numSubjects + 1:2*numSubjects, 1) = {'3'};
    mappingsExp2(2*numSubjects+1:3*numSubjects,1) = {'5'};

    y_Test = [map1, map3, map5]';

    mappings = [mappingsExp2];

    % 1-way anova - Mappings:
    [p, ~, stats] = anova1(y_Test, mappingsExp2, "display", showStats);

else
    % Groups:
    % Mapping groups
    mappingsExp2(1:...
        numSubjects*numTrialsPerMapping(p), 1) = {'1'};
    mappingsExp2(numSubjects*numTrialsPerMapping(p) + 1:...
        2*numSubjects*numTrialsPerMapping(p), 1) = {'3'};
    mappingsExp2(2*numSubjects*numTrialsPerMapping(p)+1:...
        3*numSubjects*numTrialsPerMapping(p),1) = {'5'};

    % Experiment Type condition
    experimentType2(1:3*numSubjects*numTrialsPerMapping(p), 1) = ...
        {'testing'};

    %Convert matrix of subject results to column vectors

    %CompletionTime Mapping 1 - Testingans
    y_Map1_Test = reshape([map1{:,p}],[],1);
    %CompletionTime Mapping 3 - Testing
    y_Map3_Test  = reshape([map3{:,p}],[],1);
    %CompletionTime Mapping 5 - Testing
    y_Map5_Test  = reshape([map5{:,p}],[],1);

    %Find p-values
    %vertically concatenate columns of the same metric

    disp("Test")
    % y_Test = [y_Map1_Test; y_Map3_Test; y_Map5_Test];
    y_Test = [y_Map1_Test, y_Map3_Test, y_Map5_Test];

    mappings = [mappingsExp2];
    % experimentType = [experimentType2];
    group = {mappings, subjectsAllMappings};

    % 1-way anova - Mappings:
    [p, ~, stats] = anova1(y_Test, mappingsExp2, "display", showStats);

% Create matirx of strings naming the subjects for only one of the
% mappings, to be duplicated later:
for j = 1:numSubjects
    for i = 1:numTrialsPerMapping(p)
        subjects1Mapping{i,j} = strcat('Subject #', num2str(subjectNum(j)));
    end
end

subjects1Mapping = reshape(subjects1Mapping,[],1);
% Duplicate:
subjectsAllMappings = repmat(subjects1Mapping,3,1);

end

% [p, tbl, stats] = kruskalwallis(y_Test, mappingsExp2, "on");

% 1-way anova - Subjects:
% [p, tbl, stats] = anova1(y_Test, subjectsAllMappings, "display", showStats);


% Interactable comparison with any group:
% close all;
figure;
[results,means,~,gnames] = multcompare(stats, "CriticalValueType",...
    "dunn-sidak", "Alpha", 0.05);
title(strcat(metricName," -- Interactable"));
improvePlot_v2(false, true, fontSize, width, height);

%Tables with Variable Control:
tbl = array2table([results,means],"VariableNames", ...
    ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value","Mean","Standard Error"]);
tbl.("Group A")=gnames(tbl.("Group A"));
tbl.("Group B")=gnames(tbl.("Group B"))

end