include Fox

class SprPHSceneView < FXTreeList
	def initialize(owner, phscene)
		super(owner, nil, 0, (TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_ROOT_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		@phscene = phscene
		connect(SEL_SELECTED, method(:onSelected))
		refresh
	end

	# ツリーを更新
	def refresh()
		clearItems()

		# information of active scene
		phsdk   = $sprapp.GetSdk().GetPHSdk()
		
		# scene
		@itemScene = appendItem(nil, "scene", nil, nil, @phscene)

		# solids
		solids = @phscene.GetSolids()
		@itemSolids = appendItem(nil, "solids (#{@phscene.NSolids()})")
		for i in 0..@phscene.NSolids()-1
			s = solids[i]
			appendItem(@itemSolids, s.GetName(), nil, nil, s)
		end

		# shapes
		@itemShapes = appendItem(nil, "shapes (#{phsdk.NShape()})")
		for i in 0..phsdk.NShape()-1
			shape = phsdk.GetShape(i)
			appendItem(@itemShapes, shape.GetName, nil, nil, shape)
		end

		# joints
		@itemJoints = appendItem(nil, "joints (#{@phscene.NJoints()})")
		for i in 0..@phscene.NJoints()-1
			j = @phscene.GetJoint(i)
			appendItem(@itemJoints, j.GetName(), nil, nil, j)
		end

	end

	# ツリーアイテム選択
	def onSelected(sender, sel, item)
		$propertymanager.refresh(item.data)
	end

end

class SprGRSceneView < FXTreeList
	def initialize(owner, grscene)
		super(owner, nil, 0, (TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_ROOT_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		@grscene = grscene
		connect(SEL_SELECTED, method(:onSelected))
		refresh
	end
	
	def refresh
		clearItems
		# scene graph
		world = @grscene.GetWorld()
		@itemWorld = appendItem(nil, "world (#{world.NChildren()})")
		appendChildNodes(world, @itemWorld)
	end

	# シーングラフのツリーを再帰的に構築
	def appendChildNodes(parent, item)
		children = parent.GetChildren()
		children.each do |c|
			citem = appendItem(item, c.GetName())

			# 子ノードがFrameならば再帰呼び出し
			fr = GRFrameIf.Cast(c)
			appendChildNodes(fr, citem) if fr
		end
	end

	# ツリーアイテム選択
	def onSelected(sender, sel, item)
		$propertymanager.refresh(item.data)
	end

end

class SprFWSceneView < FXTabBook
	def initialize(owner, scene)
		super(owner, nil, 0, TABBOOK_BOTTOMTABS)
		@scene = scene
		FXTabItem.new(self, '  physics  ', nil, TAB_BOTTOM)
		@phsceneview = SprPHSceneView.new(self, @scene.GetPHScene())
		FXTabItem.new(self, '  graphics  ', nil, TAB_BOTTOM)
		@grsceneview = SprGRSceneView.new(self, @scene.GetGRScene())
	end

	def refresh()
		@phsceneview.refresh
		@grsceneview.refresh
	end
end

class SprSceneView < FXTabBook
	#
	def initialize(owner)
		super(owner, nil, 0, TABBOOK_BOTTOMTABS | LAYOUT_FILL_X | LAYOUT_FILL_Y)
		@tabs = []
		@contents = []
	end

	#
	def addTab(scene)
		puts 'addtab'
		@tabs.push(FXTabItem.new(self, "  #{scene.GetName()}  ", nil, TAB_BOTTOM))
		@contents.push(SprFWSceneView.new(self, scene))
		recalc
	end

	def clear()
		children().each { |c| removeChild(c) }
		@tabs = []
		@contents = []
	end

	def refresh
		@contents[current].refresh
	end

	# Released right button
	#def onTextRightMouse(sender, sel, event)
	#	if !event.moved
	#  	pane = FXMenuPane.new(self)
	#  	FXMenuCommand.new(pane, "Undo", @undoicon, @undolist, FXUndoList::ID_UNDO)
	#  	FXMenuCommand.new(pane, "Redo", @redoicon, @undolist, FXUndoList::ID_REDO)
	#  	FXMenuSeparator.new(pane)
	#  	FXMenuCommand.new(pane, "Cut", @cuticon, @editor, FXText::ID_CUT_SEL)
	#  	FXMenuCommand.new(pane, "Copy", @copyicon, @editor, FXText::ID_COPY_SEL)
	#  	FXMenuCommand.new(pane, "Paste", @pasteicon, @editor, FXText::ID_PASTE_SEL)
	#  	FXMenuCommand.new(pane, "Select All", nil, @editor, FXText::ID_SELECT_ALL)
	#  	pane.create
	#  	pane.popup(nil, event.root_x, event.root_y)
	#  	getApp().runModalWhileShown(pane)
	#	end
	#	return 1
	#end

end
