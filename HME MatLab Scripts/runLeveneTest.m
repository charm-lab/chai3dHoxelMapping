% Hoxel Mapping Experiments (HME) Levene Test of Variance Homogeneity
% Author: Jasmin E. Palmer
function [p2, stats] = runLeveneTest(map1, map3, map5, metricName)

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

    % subjectsAllMappings = repmat(subjects1Mapping,3,1);
    
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
   
    mappings = [mappingsExp2];

    [p2, stats] = vartestn(y_Test, mappings, 'TestType','LeveneAbsolute')



    % Multiplpe comparison if p is small enough:
    if (p2(1) < 0.05)

        disp(strcat("~~~p < 0.05 --  EQUAL VAR for ", metricName))
        disp(strcat("p = ", num2str(p2(1))))

    % figure;
    % [comp,m,~,gnames] = multcompare(stats, "CriticalValueType",...
    %     "dunn-sidak", "Alpha", 0.05);
    % title(strcat(metricName," -- Interactable"));
    % improvePlot_v2(false, true, fontSize, width, height);
    % 
    % %Tables with Variable Control:
    % tbl = array2table(comp,"VariableNames", ...
    %     ["Group A","Group B","Lower Limit","A-B","Upper Limit","p-value"]);
    % tbl.("Group A")=gnames(tbl.("Group A"));
    % tbl.("Group B")=gnames(tbl.("Group B")) % Keep w/o ;
    % 
    % 
    % tbl2 = array2table(m,"RowNames", {'Mapping1', 'Mapping3', 'Control'}, ...
    %     "VariableNames",["Mean","Standard Error"])
    else
        disp(strcat("p > 0.05 -- NOT EQUAL VAR for ", metricName))
        disp(strcat("p = ", num2str(p2(1))))
        % tbl = "~";
    end
end