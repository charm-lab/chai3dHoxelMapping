% Creates bar plot with error bars
% Also has the option to include subject averages along the error bars

function [barPlot] = createBarPlot(map1, map5,...
    plotTitle, xAxisLabel, yAxisLabel, yAxisLimits)

numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');
numSubjects =  evalin('base', 'numSubjects');

markSubjectAverages = true;

for p = 1:numExperimentTypes
    % Data from each mapping from all subjects reformed into one vector:
    data1 = reshape([map1{:,p}],[],1);
    data5 = reshape([map5{:,p}],[],1);

    % Color code plot based on exp type:
    if(p == 1)
        h1 = bar(p,[mean(data1); mean(data5)]); hold on;

        % Get the x coordinate of the bars
        for i = 1:numMappings
            x(i,:) = h1(i).XEndPoints;
        end
        errorbar(x',[mean(data1);mean(data5)],[std(data1); std(data5)],...
            'ks','linestyle','none','MarkerFaceColor','k');
        % Color the bars:
        h1(1).FaceColor = [0.8 0.0 0.0];
        h1(2).FaceColor = [1.0 0.7 0.8];

        if (markSubjectAverages == true)
            % Get the mean for each subject over their trials by in a mapping:
            subjectMeans1 = mean([map1{:,p}]);
            subjectMeans5 = mean([map5{:,p}]);

            % Add them to the bar plot
            h3 = plot(x', [subjectMeans1;subjectMeans5]' ,"ko");
        end
    end
    if(p == 2)
        h2 = bar(p,[mean(data1); mean(data5)]); hold on;
        % Get the x coordinate of the bars
        for i = 1:numMappings
            x(i,:) = h2(i).XEndPoints;
        end
        errorbar(x',[mean(data1);mean(data5)],[std(data1); std(data5)],...
            'ks','linestyle','none','MarkerFaceColor','k');
        % Color the bars:
        h2(1).FaceColor = [0.2 0.2 0.7];
        h2(2).FaceColor = [0.7 0.8 0.9];

        if (markSubjectAverages == true)
            % Get the mean for each subject over their trials by in a mapping:
            subjectMeans1 = mean([map1{:,p}]);
            subjectMeans5 = mean([map5{:,p}]);

            % Add them to the bar plot
            h4 = plot(x', [subjectMeans1;subjectMeans5]' ,"ko");
        end
    end
end

ylim(yAxisLimits);
xticks([1:numExperimentTypes]);
% tickLabels = ["Color \Delta, Trial \Rightarrow",...
%     "No Color \Delta, Trial \Rightarrow"];
tickLabels = ["Training, Color \Delta", "Testing, No Color \Delta"];
set(gca,'xTick', [1:numExperimentTypes],'xticklabel', tickLabels); %#ok<NBRAK>

improvePlot_v2(false, true, 18, 1000, 700);
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);

if (markSubjectAverages == true)
    legend([h1(1), h1(2), h2(1), h2(2), h3(1)],...
        "Mapping 1", "Mapping 5",...
        "Mapping 1", "Mapping 5",...
        "Subject Averages",...
        "Location", "northwest");
else
    legend([h1(1), h1(2), h2(1), h2(2)],...
        "Mapping 1", "Mapping 5",...
        "Mapping 1", "Mapping 5",...
        "Location", "northwest");
end
hold off;

end