include Fox

class SprSceneView < FXTreeList
	def initialize(owner, scene)
		super(owner, nil, 0, (TREELIST_SHOWS_LINES|TREELIST_SHOWS_BOXES|TREELIST_ROOT_BOXES|LAYOUT_FILL_X|LAYOUT_FILL_Y))
		@scene = scene
		connect(SEL_SELECTED, method(:onSelected))
		refresh
	end

	# update tree
	def refresh()
		clearItems

		# list of Objects
		@itemObjects = appendItem(nil, "objects (#{@scene.objects.size})")
		@itemObjects.expanded = true
		@scene.objects.each {|obj|
			itemObj = appendItem(@itemObjects, obj.name, nil, nil, obj)
			itemShapes = appendItem(itemObj, "shapes (#{obj.shapes.size})")
			obj.shapes.each {|shape|
				appendItem(itemShapes, shape.name, nil, nil, shape)
			}
		}

		# list of Joints
		@itemJoints = appendItem(nil, "joints (#{@scene.joints.size})")
		@itemJoints.expanded = true
		@scene.joints.each {|joint|
			appendItem(@itemJoints, joint.name, nil, nil, joint)
		}

		# list of Shapes
		@itemShapes = appendItem(nil, "shapes (#{$sprapp.shapes.size})")
		@itemShapes.expanded = true
		$sprapp.shapes.each {|shape|
			itemShape = appendItem(@itemShapes, shape.name, nil, nil, shape)
		}

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
		$property.text = item.data.to_s
	end

end

class SprAppView < FXTabBook
	#
	def initialize(owner)
		super(owner, nil, 0, TABBOOK_BOTTOMTABS | LAYOUT_FILL_X | LAYOUT_FILL_Y)
		@tabs = []
		@contents = []
	end

	#
	def addTab(scene)
		@tabs.push(FXTabItem.new(self, "  #{scene.name}  ", nil, TAB_BOTTOM))
		@contents.push(SprSceneView.new(self, scene))
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
