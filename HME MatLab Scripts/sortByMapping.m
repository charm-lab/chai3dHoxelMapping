%** Sorts subjects form expriment by their mapping condition
%   Also assumes subjects are sorted columnwise

function sortedVariable = sortByMapping(parameter, mapping)

%Pull data from workspace
% numSubjects = evalin('base','numSubjects');
j = evalin('base','j');

sortedVariable(:,1) = parameter(mapping,j);

% for j = 1:numSubjects
%     disp(strcat("j: ", num2str(j)))
%     sortedVariable(:,j) = parameter(mapping(j,:),j)
% end

end