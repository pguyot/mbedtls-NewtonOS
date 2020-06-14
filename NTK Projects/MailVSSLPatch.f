InstallScript := func(partFrame, removeFrame)
begin
	if HasSlot(GetRoot(), '|mail:simple|) then
	begin
		local patchedImap := Clone(GetRoot().|mail:simple|._proto.IMAP_FSM);
		patchedImap.BuildConfigOptions_Connect := func()
		begin
			local options := inherited:BuildConfigOptions_Connect();
			if remotePort = 993 then
			begin
				AddArraySlot(options, {
					label: "SSL ", type: 'option, opCode: opSetRequired, result: nil, form: 'template,
					data: { arglist: [true], typelist: [kStruct, kBoolean,] }
				});
				AddArraySlot(options, {
					label: "SSLh", type: 'option, opCode: opSetRequired, result: nil, form: 'template,
					data: { arglist: [remoteAddr], typelist: [ kStruct, [kArray, kChar, 0]] }
				});
			end;
			return options;
		end;
		GetRoot().|mail:simple|.IMAP_FSM := patchedImap;
	end;
end;


RemoveScript := func(removeFrame)
begin
	if HasSlot(GetRoot(), '|mail:simple|) then
		RemoveSlot(GetRoot().|mail:simple|, 'IMAP_FSM);
	UndefGlobalVar('sslLastEx);
end;

SetPartFrameSlot('text, "Mail V SSL Patch");

SetPartFrameSlot('tapAction, func()
begin
	GetRoot():Notify( 3, "Mail V SSL Patch",
		"Quick and dirty hack to enable SSL with Mail V" );
end);
