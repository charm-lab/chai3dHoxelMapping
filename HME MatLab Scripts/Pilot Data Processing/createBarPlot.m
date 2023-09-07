function [barPlot] = createBarPlot(map1, map5,...
    plotTitle, xAxisLabel, yAxisLabel, yAxisLimits)


numMappings = evalin('base','numMappings');
numExperimentTypes = evalin('base', 'numExperimentTypes');

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
        h2(1).FaceColor = [0.3 0.6 0.1];
        h2(2).FaceColor = [0.7 0.8 0.5];
    end
    if (p == 3)
        h3 = bar(p,[mean(data1); mean(data5)]); hold on;
        % Get the x coordinate of the bars
        for i = 1:numMappings
            x(i,:) = h3(i).XEndPoints;
        end
        errorbar(x',[mean(data1);mean(data5)],[std(data1); std(data5)],...
            'ks','linestyle','none','MarkerFaceColor','k');
        % Color the bars:
        h3(1).FaceColor = [0.2 0.2 0.7];
        h3(2).FaceColor = [0.7 0.8 0.9];
    end
end

ylim(yAxisLimits);
xticks([1:3]);
tickLabels = ["Color \Delta, Trial \Rightarrow",...
    "No Color \Delta, Trial \Rightarrow",...
    "No Color \Delta, Trial \otimes"];
set(gca,'xTick', [1:3],'xticklabel', tickLabels); %#ok<NBRAK>

improvePlot_v2(false, true, 18, 1000, 700);
xlabel(xAxisLabel); ylabel(yAxisLabel);
title(plotTitle);

%     legend([h1(1), h2(1), h3(1)],...
%         "Color \Delta, Trial \Rightarrow",...
%         "No Color \Delta, Trial \Rightarrow",...
%         "No Color \Delta, Trial \otimes",...
%         "Location","northeast"); hold off;


legend([h1(1), h1(2), h2(1), h2(2), h3(1), h3(2)],...
    "Mapping 1", "Mapping 5",...
    "Mapping 1", "Mapping 5",...
    "Mapping 1", "Mapping 5",...
    "Location", "northeast"); hold off;

end