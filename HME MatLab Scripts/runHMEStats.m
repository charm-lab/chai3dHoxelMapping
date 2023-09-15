% Hoxel Mapping Experiments (HME) Anova Analysis
% Author: Jasmin E. Palmer
function [p, tbl, stats] = runHMEStats(map1, map3, map5, metricName)

numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');
numSubjects =  evalin('base', 'numSubjects');
numTrialsPerMapping =  evalin('base', 'numTrialsPerMapping');

% Statistcal Tests v2 -- N-Way ANOVA
    % 2-way ANOVA
    % p = anovan(y, group)
    showStats = "on";
    format long; % close all;
    % fontSize = 10; width = 1700; height = 1000;
    fontSize = 10; width = 700; height = 400;

    interactableCompare = true;
    % interactableCompare = false;

    for p = 1:numExperimentTypes
        if (p == 1)
            % Groups:
            % Mapping groups
            mappingsExp1(1:...
                numSubjects*numTrialsPerMapping(p), 1) = {'1'};
            mappingsExp1(numSubjects*numTrialsPerMapping(p) + 1:...
                2*numSubjects*numTrialsPerMapping(p), 1) = {'3'};
            mappingsExp1(2*numSubjects*numTrialsPerMapping(p)+1:...
                3*numSubjects*numTrialsPerMapping(p),1) = {'5'};

            % Experiment Type condition
            experimentType1(1:3*numSubjects*numTrialsPerMapping(p), 1) = ...
                {'training'};

        else % (p == 2)
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

    %Convert matrix of subject results to column vectors
    p=1;
    %CompletionTime Mapping 1 - Training
    y_Map1_Train = reshape([map1{:,p}],[],1);
    %CompletionTime Mapping 3 - Training
    y_Map3_Train  = reshape([map3{:,p}],[],1);
    %CompletionTime Mapping 5 - Training
    y_Map5_Train  = reshape([map5{:,p}],[],1);

    p=2;
    %CompletionTime Mapping 1 - Testing
    y_Map1_Test = reshape([map1{:,p}],[],1);
    %CompletionTime Mapping 3 - Testing
    y_Map3_Test  = reshape([map3{:,p}],[],1);
    %CompletionTime Mapping 5 - Testing
    y_Map5_Test  = reshape([map5{:,p}],[],1);
    
    %Find p-values
    %vertically concatenate columns of the same metric
    
    % disp("Train")
    % y_Train = [y_Map1_Train; y_Map3_Train; y_Map5_Train];
    % % aov = anova({mappingsExp1}, y_Train, FactorNames=["mappings"])

    disp("Test")
    y_Test = [y_Map1_Test; y_Map3_Test; y_Map5_Test];
    % aov = anova({mappingsExp2}, yCT_Test, FactorNames=["mappings"])
   
    mappings = [mappingsExp2];
    experimentType = [experimentType2];
    group = {mappings, experimentType};

    [p, tbl, stats] = anova1(y_Test, mappingsExp2, "display", showStats);
    
    % disp("Both")
    % % y = [y_Train; y_Test];
    % y = [y_Test];
    % % mappings = [mappingsExp1; mappingsExp2];
    % mappings = [mappingsExp2];
    % % experimentType = [experimentType1; experimentType2];
    % experimentType = [ experimentType2];
    % group = {mappings, experimentType};
    % 
    % [p, tbl, stats] = anovan(y, group, "Model","interaction",...
    %     "Varnames",["mappings","experimentType"],...
    %     "display", showStats);

    % Compare signifcance with control group only:
    if(interactableCompare == false)
        % close all;
        figure;
        %Completion Time 
        comp = multcompare(stats, "CriticalValueType", "dunnett", ...
            "ControlGroup", 3, "Alpha", 0.05);
        title(strcat(metricName," -- Control -- Dunnett Test"));
        improvePlot_v2(false, true, fontSize, width, height);

        %Tables with Mapping 5 as Control:    
        tbl = array2table(comp,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"]);

    % Interactable comparison with any group:
    else    
        % close all;
        figure;
        comp = multcompare(stats);
        title(strcat(metricName," -- Interactable"));
        improvePlot_v2(false, true, fontSize, width, height);

        %Tables with Variable Control:    
        tbl = array2table(comp,"VariableNames", ...
        ["Group A","Group B","Lower Limit","A-B","Upper Limit","P-value"]);
    end   
end