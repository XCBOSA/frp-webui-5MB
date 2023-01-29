(function framework7ComponentLoader(e,t){void 0===t&&(t=!0);var a=e.$,l=e.utils,s=(e.getDevice,e.getSupport,e.Class),c=(e.Modal,e.ConstructorMethods),d=(e.ModalMethods,e.$jsx,l.extend),o=l.deleteProps;class n extends s{constructor(e,t){void 0===t&&(t={}),super(t,[e]);const l=this,s={};l.useModulesParams(s),l.params=d(s,t);const c=a(l.params.el);if(0!==c.length){if(l.$el=c,l.el=c[0],l.$el[0].f7DataTable){const e=l.$el[0].f7DataTable;return l.destroy(),e}return l.$el[0].f7DataTable=l,d(l,{collapsible:c.hasClass("data-table-collapsible"),$headerEl:c.find(".data-table-header"),$headerSelectedEl:c.find(".data-table-header-selected")}),l.attachEvents=function(){l.$el.on("change",'.checkbox-cell input[type="checkbox"]',o),l.$el.find("thead .sortable-cell").on("click",n)},l.detachEvents=function(){l.$el.off("change",'.checkbox-cell input[type="checkbox"]',o),l.$el.find("thead .sortable-cell").off("click",n)},l.useModules(),l.init(),l}function o(e){if(e.detail&&e.detail.sentByF7DataTable)return;const t=a(this),s=t[0].checked,d=t.parents("td,th").index();if(t.parents("thead").length>0)0===d&&c.find("tbody tr")[s?"addClass":"removeClass"]("data-table-row-selected"),c.find(`tbody tr td:nth-child(${d+1}) input`).prop("checked",s).trigger("change",{sentByF7DataTable:!0}),t.prop("indeterminate",!1);else{0===d&&t.parents("tr")[s?"addClass":"removeClass"]("data-table-row-selected");const e=c.find(`tbody .checkbox-cell:nth-child(${d+1}) input[type="checkbox"]:checked`).length,a=c.find("tbody tr").length,l=c.find(`thead .checkbox-cell:nth-child(${d+1}) input[type="checkbox"]`);s?e===a&&l.prop("checked",!0).trigger("change",{sentByF7DataTable:!0}):l.prop("checked",!1),l.prop("indeterminate",e>0&&e<a)}l.checkSelectedHeader()}function n(){const e=a(this),t=e.hasClass("sortable-cell-active"),s=e.hasClass("sortable-desc")?"desc":"asc";let d;t?(d="desc"===s?"asc":"desc",e.removeClass("sortable-desc sortable-asc").addClass(`sortable-${d}`)):(c.find("thead .sortable-cell-active").removeClass("sortable-cell-active"),e.addClass("sortable-cell-active"),d=s),e.trigger("datatable:sort",d),l.emit("local::sort dataTableSort",l,d)}}setCollapsibleLabels(){const e=this;e.collapsible&&e.$el.find("tbody td:not(.checkbox-cell)").each((t=>{const l=a(t),s=l.index(),c=l.attr("data-collapsible-title");c||""===c||l.attr("data-collapsible-title",e.$el.find("thead th").eq(s).text())}))}checkSelectedHeader(){const e=this;if(e.$headerEl.length>0&&e.$headerSelectedEl.length>0){const t=e.$el.find("tbody .checkbox-cell input:checked").length;e.$el[t>0?"addClass":"removeClass"]("data-table-has-checked"),e.$headerSelectedEl.find(".data-table-selected-count").text(t)}}init(){const e=this;e.attachEvents(),e.setCollapsibleLabels(),e.checkSelectedHeader()}destroy(){let e=this;e.$el.trigger("datatable:beforedestroy"),e.emit("local::beforeDestroy dataTableBeforeDestroy",e),e.attachEvents(),e.$el[0]&&(e.$el[0].f7DataTable=null,delete e.$el[0].f7DataTable),o(e),e=null}}var i={name:"dataTable",static:{DataTable:n},create(){this.dataTable=c({defaultSelector:".data-table",constructor:n,app:this,domProp:"f7DataTable"})},on:{tabBeforeRemove(e){const t=this;a(e).find(".data-table-init").each((e=>{t.dataTable.destroy(e)}))},tabMounted(e){const t=this;a(e).find(".data-table-init").each((e=>{t.dataTable.create({el:e})}))},pageBeforeRemove(e){const t=this;e.$el.find(".data-table-init").each((e=>{t.dataTable.destroy(e)}))},pageInit(e){const t=this;e.$el.find(".data-table-init").each((e=>{t.dataTable.create({el:e})}))}},vnode:{"data-table-init":{insert(e){const t=e.elm;this.dataTable.create({el:t})},destroy(e){const t=e.elm;this.dataTable.destroy(t)}}}};if(t){if(e.prototype.modules&&e.prototype.modules[i.name])return;e.use(i),e.instance&&(e.instance.useModuleParams(i,e.instance.params),e.instance.useModule(i))}return i}(Framework7, typeof Framework7AutoInstallComponent === 'undefined' ? undefined : Framework7AutoInstallComponent))
