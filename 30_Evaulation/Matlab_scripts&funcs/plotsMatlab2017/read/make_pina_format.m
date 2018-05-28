function  d_data = make_pina_format(data,h_data)
if strcmp('PINA TRAJECTORY',h_data.type) == 1
    if strcmp('LLH',h_data.positionFormat) == 1
        
        d_data.t_time = [data(:,1) data(:,2)];
        d_data.lat = data(:,3);
        d_data.lon  = data(:,4);
        d_data.h = data(:,5);
        
        j = 1;
        k =[0 0 0 0 0 0 0 0]; % mely oszlopokban van NaN
        for i=1:length(data(1,:))
           if max(isnan(data(:,i))) < 1
            buff(:,i)= data(:,i);
           end
           j=j+1;
        end
        data = buff;
      
        
        
        
           
           
    %ha hiányoznak a roll, pitch yaw adatok akkor, akkor a t_data-ban ezek az
    %elemek legyenek "empty" OK
    size_of_d_data = size(data);
        if size_of_d_data(2) == 5 
            d_data.roll  = [];
            d_data.pitch = [];
            d_data.yaw   = [];
        else
            d_data.roll  = data(:,6);
            d_data.pitch = data(:,7);
            d_data.yaw   = data(:,8);
            d_data.format = h_data.positionFormat;
        end
    end

    if strcmp('ECEF',h_data.positionFormat) == 1

        d_data.t_time = [data(:,1) data(:,2)];

        d_data.x = data(:,3);
        d_data.y = data(:,4);
        d_data.z = data(:,5);

        size_of_d_data = size(data);
        if size_of_d_data(2) == 5 
            d_data.roll  = [];
            d_data.pitch = [];
            d_data.yaw   = [];
        else
            d_data.roll  = data(:,6);
            d_data.pitch = data(:,7);
            d_data.yaw   = data(:,8);
            d_data.format = h_data.positionFormat;
        end
    end
end
           
if strcmp('PINA IMU BODY DATA',h_data.type) == 1
        d_data.t_time = [data(:,1) data(:,2)];

        d_data.ax = data(:,3);
        d_data.ay = data(:,4);
        d_data.az = data(:,5);

        size_of_d_data = size(data);
        if size_of_d_data(2) == 5 
            d_data.wx  = [];
            d_data.wy = [];
            d_data.wz  = [];
        else
            d_data.wx  = data(:,6);
            d_data.wy = data(:,7);
            d_data.wz   = data(:,8);
            d_data.format = h_data.positionFormat;
        end
end

end 