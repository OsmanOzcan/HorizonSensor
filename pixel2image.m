clear;
clc;
close all;

r = rateControl(1);
reset(r);

while(1)
    data = readtable('boundaryPoints.txt');
    data = data{:,:};
    flag = data(1,1);
    points = data(2:end,:);
    
    if(flag == 0)
        plot(points(:,2), 32-points(:,1), "o");
        xlim([0 32]);
        ylim([0 32]);
    
    elseif(flag == 1)
        plot(points(:,2), points(:,1), "o");
        xlim([0 32]);
        ylim([0 32]);
    
    elseif(flag == 2)
        plot(32-points(:,1), 32-points(:,2), "o");
        xlim([0 32]);
        ylim([0 32]);
    
    elseif(flag == 3)
        plot(points(:,1), 32-points(:,2), "o");
        xlim([0 32]);
        ylim([0 32]);
    end

    waitfor(r);
end


