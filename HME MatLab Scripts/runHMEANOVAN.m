% Hoxel Mapping Experiments (HME) Anova Analysis
% Author: Jasmin E. Palmer
function [p2, tbl, stats] = runHMEANOVAN(map1, map3, map5, metricName)

numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');
numSubjects =  evalin('base', 'numSubjects');
subjectNum = evalin('base','subjectNum');
numTrialsPerMapping =  evalin('base', 'numTrialsPerMapping');

% Statistcal Tests v3 -- 2-Way ANOVA
    % p = anovan(y, group)
    showStats = "on";
    format long; % close all;
    % fontSize = 10; width = 1700; height = 1000;
    fontSize = 10; width = 700; height = 400;

    interactableCompare = true;
    % interactableCompare = false;

    for p = 1:numExperimentTypes
        if (p == 2)
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
        end
    end

    % Create matirx of strings naming the subjects for only one of the 
    % mappings, to be duplicated later:
    for j = 1:numSubjects
        for i = 1:numTrialsPerMapping(p)
            subjects1Mapping{i,j} = strcat('Subject #', num2str(subjectNum(j)));
        end
    end

    subjects1Mapping = reshape(subjects1Mapping,[],1);

    subjectsAllMappings = repmat(subjects1Mapping,3,1);
    
    %Convert matrix of subject results to column vectors
    p=2;
    %CompletionTime Mapping 1 - Testingans
    y_Map1_Test = reshape([map1{:,p}],[],1);
    %CompletionTime Mapping 3 - Testing
    y_Map3_Test  = reshape([map3{:,p}],[],1);
    %CompletionTime Mapping 5 - Testing
    y_Map5_Test  = reshape([map5{:,p}],[],1);

    %Find p-values
    %vertically concatenate columns of the same metric
   
    disp("Test")
    y_Test = [y_Map1_Test; y_Map3_Test; y_Map5_Test];
    % aov = anova({mappingsExp2}, yCT_Test, FactorNames=["mappings"])
   
    mappings = [mappingsExp2];
    % experimentType = [experimentType2];
    group = {mappings, subjectsAllMappings};

    % n-way anova with n = 2:
    [p2, ~, stats] = anovan(y_Test, group, "Model","full",...
        "Varnames", ["Mappings", "Subjects"],...
        "display", showStats);

    
    % Multiplpe comparison if p is small enough:
    if (p2(1) < 0.05)
  
    figure;
    [results,means,~,gnames] = multcompare(stats, "CriticalValueType",...
        "dunn-sidak", "Alpha", 0.05);
    title(strcat(metricName," -- Interactable"));
    improvePlot_v2(false, true, fontSize, width, height);

    % %Tables with Variable Control:
    % tbl = array2table(results,"VariableNames", ...
    %     ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"]);
    % tbl.("Group A")=gnames(tbl.("Group A"));
    % tbl.("Group B")=gnames(tbl.("Group B")) % Keep w/o ;
    % 
    % 
    % tbl2 = array2table(means,"RowNames",gnames, ...
    %     "VariableNames",["Mean","Standard Error"])

    tbl = array2table([results,means],"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value","Mean","Standard Error"]);
    tbl.("Group A")=gnames(tbl.("Group A"));
    tbl.("Group B")=gnames(tbl.("Group B"))

    else
        disp(strcat("p > 0.05 -- NO MULTCOMP for ", metricName))
        disp(strcat("p = ", num2str(p2(1))))
        tbl = "~";
    end
end